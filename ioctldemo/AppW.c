#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "myioctl.h"
#define SIZE 16

int main()
{
	int fd,ret;
	char ubuffW[]="Fromuser";

	fd=open("/dev/first_dev",O_RDWR);
	if(fd<0)
	{
		perror("ERROR\n");
		return fd;
	}
	ret = ioctl(fd,CMD_1);
	ret = write(fd,ubuffW,sizeof(ubuffW));
	printf("Writen %d bytes\n",ret);
	return 0;
}

