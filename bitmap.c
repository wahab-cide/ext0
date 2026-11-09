#include "ext0.h"

static int get_bit(uint8_t *map, uint64_t i) {
	return (map[i / 8] >> (i % 8)) & 1;
}

static void set_bit(uint8_t *map, uint64_t i) {
	map[i / 8] |= 1 << (i % 8);
}

static void clear_bit(uint8_t *map, uint64_t i) {
	map[i / 8] &= ~(1 << (i % 8));
}

/*
 * Find the first 0 among the first `count` bits of the bitmap at block
 * `start`, set it, and save that block back to disk.
 * Returns the bit number, or -1 if every bit is taken.
 */
static int64_t alloc_bit(lba_t start, uint64_t count) {
	uint8_t block[BLOCK_SIZE];

	for (uint64_t i = 0; i < count; i++) {
		uint64_t bit = i % BITS_PER_BLOCK;
		lba_t lba = start + i / BITS_PER_BLOCK;

		if (bit == 0) {
			read_blocks(block, 1, lba);
		}
		if (!get_bit(block, bit)) {
			set_bit(block, bit);
			write_blocks(block, 1, lba);
			return i;
		}
	}
	return -1;
}

static void free_bit(lba_t start, uint64_t i) {
	uint8_t block[BLOCK_SIZE];
	lba_t lba = start + i / BITS_PER_BLOCK;

	read_blocks(block, 1, lba);
	clear_bit(block, i % BITS_PER_BLOCK);
	write_blocks(block, 1, lba);
}

/* Returns a free inode number, or -1 if there are none. */
int64_t alloc_inode(void) {
	return alloc_bit(sb.i_bitmap_start, sb.num_inodes);
}

void free_inode(uint64_t inum) {
	free_bit(sb.i_bitmap_start, inum);
}

/* Returns the disk block number of a free data block, or -1 if full. */
int64_t alloc_block(void) {
	int64_t i = alloc_bit(sb.d_bitmap_start, sb.num_data_blocks);

	if (i < 0) {
		return -1;
	}
	return sb.d_region_start + i;
}

void free_block(lba_t lba) {
	free_bit(sb.d_bitmap_start, lba - sb.d_region_start);
}
