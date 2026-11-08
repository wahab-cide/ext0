# ext0

A small ext2-style file system built with FUSE.

## Build and run (Docker)

```
docker build -t ext0-dev .
docker run -it --rm --device /dev/fuse --cap-add SYS_ADMIN \
	--security-opt apparmor:unconfined -v "$PWD":/ext0 ext0-dev
make
mkdir -p mnt && ./ext0 mnt     # mount
fusermount -u mnt              # unmount
```
