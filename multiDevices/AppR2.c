#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define SIZE 16

int main()
{
	int fd,ret;
	char ubuffR[SIZE];

	fd=open("/dev/second_dev",O_RDWR);
	if(fd<0)
	{
		perror("ERROR\n");
		return fd;
	}

	ret = read(fd,ubuffR,SIZE);
	printf("Read:%s No of bytes read %d\n",ubuffR,ret);
	return 0;
}

