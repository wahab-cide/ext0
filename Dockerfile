FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
	build-essential gdb pkg-config libfuse-dev fuse \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /ext0
