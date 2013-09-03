#define FUSE_USE_VERSION 26

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>

#include <fuse.h>

#include "raidconf.h"

#include <sys/stat.h>

unsigned long long raidsize;
RaidConf rc;
int bits;
static const char* raid_path = "/raid";
int* filehandles;

static int raid_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, raid_path) == 0) {
		stbuf->st_mode = S_IFREG | 0755;
		stbuf->st_nlink = 1;
		stbuf->st_size = raidsize;
	} else
		res = -ENOENT;

	return res;
}

int raid_open(const char *path, struct fuse_file_info *fi)
{
	int retstat = 0;
	int fd = 0;

	if (strcmp(path, raid_path) != 0) return -ENOENT;

	for(int i = 0; i < rc.Devices; i++) {
		filehandles[i] = open(rc.Entries[i].Device, fi->flags);
		fd = filehandles[i] < fd ? filehandles[i] : fd;
	}

	if (fd < 0)
		retstat = -errno;
	fi->fh = fd;
	return 0;
}

int raid_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	int retstat = 0;

	if (strcmp(path, raid_path) != 0) return -ENOENT;
	//TODO: Implement.
	//retstat = pread(fi->fh, buf, size, offset);

	return retstat;
}

int raid_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	int retstat = 0;
	if (strcmp(path, raid_path) != 0) return -ENOENT;
	int o = 0;
	long block = offset / rc.Chunk;
	long  end = offset + size;
	while(block * rc.Chunk <= end) {
		int bit = 1 << (block % bits);
		int s = block * rc.Chunk > s ? s % rc.Chunk : 0;
		int e = block * rc.Chunk < e ? e % rc.Chunk : rc.Chunk - 1;
		int l = s - e;

		block++;
	}
	retstat = pwrite(fi->fh, buf, size, offset);
	return retstat;
}


int raid_statfs(const char *path, struct statvfs *statv)
{
	int retstat = 0;
	//retstat = statvfs(fpath, statv);
	//if (retstat < 0) retstat = bb_error("bb_statfs statvfs");
	return retstat;
}

int main(int argc, char* argv[]) {
	if(argc == 1) return 0;
	rc = loadraidconf(argv[1]);
	unsigned long long minsize = LONG_MAX;
	//printf("%i, %i, %lld, %i", rc.Devices, rc.Size, rc.Chunk, rc.Save);
	for(int i = 0; i < rc.Devices; i++) {
		struct stat st;
		stat(rc.Entries[i].Device, &st);
		minsize = minsize < st.st_size ? minsize : st.st_size;
	}
	bits = rc.Size * rc.Save;
	raidsize = (minsize / rc.Chunk / rc.Size) * bits * rc.Chunk;
	filehandles = malloc(sizeof(int) * rc.Devices);
	//printf("%i, %lld: %llu\n", rc.Size, rc.Chunk, raidsize);
	return 0;
}

