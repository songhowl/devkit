#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static char serial_number[100] = { 0 };

extern const char* get_cpu_serial_number(void)
{
    if (serial_number[0] == 0)
    {
        FILE* file;
        char* chp, *end;
        char* whitespace;
        int length;

        // read serial number from /proc/cpuinfo
        file = fopen("/proc/cpuinfo", "r");
        if (! file)
            return NULL;

        while ((chp = fgets(serial_number, sizeof(serial_number), file)) != NULL)
        {
            // look for something like "Serial          : 999206122a03591c"

            if (strncmp(chp, "Serial", 6) != 0)
                continue;

            chp = strchr(chp, ':');
            if (!chp)
                continue;

             // skip colon and whitespace
            while ( *(++chp) == ' ') {}

            // truncate trailing whitespace
            end = chp;
            while (*end && *end != ' ' && *end != '\t' && *end != '\n' && *end != '\r')
                ++end;
            *end = 0;

            whitespace = strchr(chp, ' ');
            if (whitespace)
                *whitespace = 0;
            whitespace = strchr(chp, '\t');
            if (whitespace)
                *whitespace = 0;
            whitespace = strchr(chp, '\r');
            if (whitespace)
                *whitespace = 0;
            whitespace = strchr(chp, '\n');
            if (whitespace)
                *whitespace = 0;
            // shift serial number to beginning of the buffer
            memmove(serial_number, chp, strlen(chp) + 1);
            break;
        }

        fclose(file);
    }

    return (serial_number[0] ? serial_number : NULL);
}

void main(void)
{
	printf("+++%s\n", get_cpu_serial_number());
}
