#ifndef EXT0_H
#define EXT0_H

#include <stdint.h>
#include <sys/types.h>

typedef uint64_t lba_t;

#define BLOCK_SIZE 4096
#define DISK_PATH "./ext0_disk"
#define DISK_BLOCKS 2560 /* 2560 * 4096 = 10 MiB */

#define EXT0_MAGIC 0xE0E0E0E0
#define NUM_INODES 1024
#define INODE_SIZE 256
#define INODES_PER_BLOCK (BLOCK_SIZE / INODE_SIZE)
#define BITS_PER_BLOCK (BLOCK_SIZE * 8)
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

/* Where each region starts on disk; stored in block 0. */
struct superblock {
	uint32_t magic;
	uint32_t block_size;
	uint64_t num_inodes;
	lba_t i_bitmap_start;
	lba_t i_table_start;
	uint64_t num_data_blocks;
	lba_t d_bitmap_start;
	lba_t d_region_start;
};

/* layout.c */
void init_super(struct superblock *sb);
void dump_super(struct superblock *sb);

/* disk.c */
int open_disk(const char *path);
void close_disk(void);
ssize_t read_blocks(void *buf, size_t count, lba_t lba);
ssize_t write_blocks(void *buf, size_t count, lba_t lba);

#endif
