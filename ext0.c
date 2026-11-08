#define FUSE_USE_VERSION 26

#include <fuse.h>

/*
 * The table of operations FUSE calls into. It is empty for now, so
 * every request from the kernel fails with "Function not implemented".
 */
static struct fuse_operations ext0_operations = {
};

int main(int argc, char *argv[]) {
	return fuse_main(argc, argv, &ext0_operations, NULL);
}
