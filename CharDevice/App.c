#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd,ret;
	fd=open("/dev/first_dev",O_RDWR);
	if(fd<0)
	{
		perror("Error opening\n");
		return fd;
	}
	return 0;
}
