#include <fcntl.h>
#include <unistd.h>
#include "ext0.h"

static int disk_fd = -1;

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
