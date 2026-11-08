#include <assert.h>
#include <stdio.h>
#include "ext0.h"

/*
 * Print the layout and check that the regions are in order and
 * that the data region ends exactly at the last disk block.
 */
int main(void) {
	struct superblock sb;
	init_super(&sb);
	dump_super(&sb);

	assert(sb.i_bitmap_start < sb.i_table_start);
	assert(sb.i_table_start < sb.d_bitmap_start);
	assert(sb.d_bitmap_start < sb.d_region_start);
	assert(sb.d_region_start + sb.num_data_blocks == DISK_BLOCKS);

	printf("layout tests passed\n");
	return 0;
}
