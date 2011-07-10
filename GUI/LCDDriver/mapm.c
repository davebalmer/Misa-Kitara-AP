#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "mapm.h"

void* mapm(unsigned int addr, unsigned int len)
{
	int fd;
	void *result;
	fd = open("/dev/mem", O_SYNC | O_RDWR);
	if(fd == -1)
		return ((void *)addr);
	result = fmapm(fd, addr, len);
	close(fd);
	return result;
}

void* fmapm(int fd, unsigned int addr, unsigned int len)
{
	int pagesize;
	int aligned_addr;
	int aligned_len;
	void *result;
	pagesize = getpagesize();
	aligned_len = len+((pagesize-1)&addr);
	aligned_addr = addr-((pagesize-1)&addr);
	result = mmap(((void *)0),aligned_len,3,1,fd,aligned_addr);
	if(result!=((void *)-1))
	{
		return ((char*)result+((pagesize-1)&addr));
	}
	else
		return result;
}

int unmapm(void *addr, unsigned int len)
{
	int pagesize;
	pagesize = getpagesize();
	return munmap((char*)addr-((pagesize-1)&((int)addr)),len+((pagesize-1)&((int)addr)));
}

