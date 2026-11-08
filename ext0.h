#ifndef EXT0_H
#define EXT0_H

#include <stdint.h>
#include <sys/types.h>

typedef uint64_t lba_t;

#define BLOCK_SIZE 4096
#define DISK_PATH "./ext0_disk"
#define DISK_BLOCKS 2560 /* 2560 * 4096 = 10 MiB */

/* disk.c */
int open_disk(const char *path);
void close_disk(void);
ssize_t read_blocks(void *buf, size_t count, lba_t lba);
ssize_t write_blocks(void *buf, size_t count, lba_t lba);

#endif
