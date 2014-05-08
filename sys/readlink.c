#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

void read_link(void )
{
	struct stat st;
	char pathbuf[128];
	char link[256];
	char *devpath = NULL;
	char *slash;
	ssize_t link_len;
	
	stat("/dev/usbmon0",&st);
	snprintf(pathbuf, sizeof(pathbuf), "/sys/dev/char/%d:%d",
			major(st.st_rdev), minor(st.st_rdev));
	link_len = readlink(pathbuf, link, sizeof(link) - 1);
	if (link_len > 0) {
		link[link_len] = '\0';
		slash = strrchr(link, '/');
		if (slash) {
			snprintf(pathbuf, sizeof(pathbuf),
					"usb:%s", slash + 1);
			devpath = pathbuf;
		}
	}
	printf("path is %s, %s\n",devpath,link);


}

void main(void)
{
	read_link();
}
