#define FUSE_USE_VERSION 26

#include <fcntl.h>
#include <fuse.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ext0.h"

static int disk_fd = -1;
struct superblock sb;

/* ---------------- Disk I/O ---------------- */

/*
 * Open the disk file, creating it if needed, and make sure it is
 * DISK_BLOCKS blocks long. A new disk reads back as all zeros.
 * Returns 0 on success, -1 on error.
 */
int open_disk(const char *path) {
	disk_fd = open(path, O_RDWR | O_CREAT, 0644);
	if (disk_fd < 0) {
		return -1;
	}
	return ftruncate(disk_fd, DISK_BLOCKS * BLOCK_SIZE);
}

void close_disk(void) {
	close(disk_fd);
}

/*
 * Read `count` blocks starting at block `lba` into buf.
 * Returns the number of whole blocks read.
 */
ssize_t read_blocks(void *buf, size_t count, lba_t lba) {
	ssize_t n = pread(disk_fd, buf, count * BLOCK_SIZE, lba * BLOCK_SIZE);
	return n / BLOCK_SIZE;
}

/*
 * Write `count` blocks from buf to the disk, starting at block `lba`.
 * Returns the number of whole blocks written.
 */
ssize_t write_blocks(void *buf, size_t count, lba_t lba) {
	ssize_t n = pwrite(disk_fd, buf, count * BLOCK_SIZE, lba * BLOCK_SIZE);
	return n / BLOCK_SIZE;
}

/* ---------------- Disk layout ---------------- */

/*
 * Divide the disk into regions, in order: superblock, inode bitmap,
 * inode table, data bitmap, data blocks. The data region gets every
 * block left over after the others.
 */
void init_super(struct superblock *sb) {
	uint64_t i_bitmap_blocks = DIV_ROUND_UP(NUM_INODES, BITS_PER_BLOCK);
	uint64_t i_table_blocks = DIV_ROUND_UP(NUM_INODES, INODES_PER_BLOCK);

	sb->magic = EXT0_MAGIC;
	sb->block_size = BLOCK_SIZE;
	sb->num_inodes = NUM_INODES;
	sb->i_bitmap_start = 1;
	sb->i_table_start = sb->i_bitmap_start + i_bitmap_blocks;
	sb->d_bitmap_start = sb->i_table_start + i_table_blocks;

	uint64_t left = DISK_BLOCKS - sb->d_bitmap_start;
	uint64_t d_bitmap_blocks = DIV_ROUND_UP(left, BITS_PER_BLOCK);
	sb->d_region_start = sb->d_bitmap_start + d_bitmap_blocks;
	sb->num_data_blocks = DISK_BLOCKS - sb->d_region_start;
}

void dump_super(struct superblock *sb) {
	printf("magic           %x\n", sb->magic);
	printf("block size      %u\n", sb->block_size);
	printf("inodes          %" PRIu64 "\n", sb->num_inodes);
	printf("inode bitmap    %" PRIu64 "\n", sb->i_bitmap_start);
	printf("inode table     %" PRIu64 "\n", sb->i_table_start);
	printf("data bitmap     %" PRIu64 "\n", sb->d_bitmap_start);
	printf("data region     %" PRIu64 "\n", sb->d_region_start);
	printf("data blocks     %" PRIu64 "\n", sb->num_data_blocks);
}

/* ---------------- Format ---------------- */

/*
 * Lay out a fresh file system: zero both bitmaps so everything is
 * free, then write the superblock to block 0.
 */
void format_disk(void) {
	char block[BLOCK_SIZE] = {0};

	init_super(&sb);
	for (lba_t i = sb.i_bitmap_start; i < sb.i_table_start; i++) {
		write_blocks(block, 1, i);
	}
	for (lba_t i = sb.d_bitmap_start; i < sb.d_region_start; i++) {
		write_blocks(block, 1, i);
	}
	memcpy(block, &sb, sizeof(sb));
	write_blocks(block, 1, 0);
}

/*
 * Read the superblock from block 0. If the magic number is wrong the
 * disk was never formatted, so format it now.
 */
void load_super(void) {
	char block[BLOCK_SIZE];

	read_blocks(block, 1, 0);
	memcpy(&sb, block, sizeof(sb));
	if (sb.magic != EXT0_MAGIC) {
		format_disk();
	}
}

/* ---------------- FUSE operations ---------------- */

/*
 * The table of operations FUSE calls into. It is empty for now, so
 * every request from the kernel fails with "Function not implemented".
 */
static struct fuse_operations ext0_operations = {
};

int main(int argc, char *argv[]) {
	if (open_disk(DISK_PATH) < 0) {
		perror("open_disk");
		return 1;
	}
	load_super();

	int ret = fuse_main(argc, argv, &ext0_operations, NULL);
	close_disk();
	return ret;
}
