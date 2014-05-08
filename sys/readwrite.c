
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>

#define ERROR printf
static int read_data(const char *path, void *outdata, int size)
{
        int fd;
        int ret;
        if(!outdata ||(size <= 0))
        {
                return -1;
        }
        fd = open(path, O_RDONLY);
        if(fd < 0)
        {
                ERROR("Error(%d) in opening %s:%s\n", errno, path, strerror(errno));
                return -1;
        }
        ret = read(fd, outdata, size);
        close(fd);
        if (ret != size)
        {
                ERROR("Error to read %s\n", path);
                return -1;
        }
        return 0;
}

static int write_data(const char *path, const void *indata, int size)
{
        int fd;
        int ret;
        if(!indata ||(size <= 0))
        {
                return -1;
        }
        fd = open(path, O_WRONLY|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
        if (fd < 0)
        {
                ERROR("Error(%d) in opening %s:%s\n", errno, path, strerror(errno));
                return -1;
        }
        ret = write(fd, indata, size);
        close(fd);
        if(ret != size)
        {
                ERROR("Error to write data to %s\n", path);
                return -1;
        }
        return 0;
}

int write_data_to_path(const void *data, const char *dest_path, int size)
{

        if(!data || !dest_path || (size <= 0))
                return -1;

        if(write_data(dest_path, data, size))
        {
                ERROR("Failed to write data to %s\n", dest_path);
                return -1;
        }
        return 0;
}



void main(void)
{
write_data("hello","123456789",9);
}
