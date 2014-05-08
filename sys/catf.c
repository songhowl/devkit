#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>



int  fn(char * filename, char *data)
{
  int fd;
    fd = open(filename, O_RDONLY);
    if (fd >= 0) {
        int n = read(fd, data, 1023);
        if (n < 0) n = 0;

        /* get rid of trailing newline, it happens */
        if (n > 0 && data[n-1] == '\n') n--;

        data[n] = 0;
        close(fd);
	return 0;
    } else {
	return -1;
        data[0] = 0;
    }


}


void main(void)
{
char data[1024];
	fn("/proc/cmdline",data);
	printf("cmd:%s\n",data);

}
