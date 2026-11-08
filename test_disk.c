#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "ext0.h"

/*
 * Write one block, read it back, and check that its neighbor is
 * untouched. Run with `make test`.
 */
int main(void) {
	char out[BLOCK_SIZE], in[BLOCK_SIZE], zeros[BLOCK_SIZE];

	assert(open_disk("./test_disk.img") == 0);

	memset(out, 'A', BLOCK_SIZE);
	memset(zeros, 0, BLOCK_SIZE);
	assert(write_blocks(out, 1, 5) == 1);

	assert(read_blocks(in, 1, 5) == 1);
	assert(memcmp(in, out, BLOCK_SIZE) == 0);

	assert(read_blocks(in, 1, 6) == 1);
	assert(memcmp(in, zeros, BLOCK_SIZE) == 0);

	close_disk();
	printf("disk tests passed\n");
	return 0;
}
