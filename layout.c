#include <inttypes.h>
#include <stdio.h>
#include "ext0.h"

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
