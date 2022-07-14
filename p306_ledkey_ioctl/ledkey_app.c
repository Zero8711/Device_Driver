#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "ioctl_test.h"

#define DEVICE_FILENAME  "/dev/ledkey"

int main()
{
    int dev;
    char buff = 15;
    int ret;
    int key_old = 0;

    dev = open( DEVICE_FILENAME, O_RDWR|O_NDELAY );
	if(dev<0)
	{
		perror("open()");
		return 1;
	}
	ret = ioctl(dev,IOCTLTEST_KEYLEDINIT);

	ret = ioctl(dev,IOCTLTEST_LEDON);
	sleep(1);
	ret = ioctl(dev,IOCTLTEST_LEDOFF);
	sleep(1);
	ret = ioctl(dev,IOCTLTEST_LEDON);
	sleep(1);
	ret = ioctl(dev,IOCTLTEST_LEDOFF);

	ret = ioctl(dev,IOCTLTEST_KEYLEDFREE);

/*

    ret = write(dev,&buff,sizeof(buff));
	if(ret < 0)
		perror("write()");
	buff = 0;
	do {
    	ret = read(dev,&buff,sizeof(buff));              
  		if(ret < 0)
  			perror("read()");
		if(buff != key_old)
		{
			if(buff)
			{
				printf("key_no : %d\n",buff);
				write(dev,&buff,sizeof(buff));
			}
			if(buff == 8)
				break;
			key_old = buff;
		}
	} while(1);
*/
    close(dev);
    return 0;
}
