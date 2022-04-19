#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void usage(void){
    fprintf(stderr, "Usage :\n  uart $serial_dev\n");
    exit(1);
}

/* read from uart, write to uart */
int main(int argc, char* argv[]){
    char *uart_name;
    int fd; // uart fd
    char buf[256];

    if (argc != 2){
        usage();
    }
    uart_name = argv[1];

    fd = open(uart_name, O_RDWR);
    if (fd < 0) {
        printf("failed to open %s: %d\n", uart_name, fd);
        return -1;
    }
    
    while (1)
    {
        unsigned long len = read(fd, buf, sizeof(buf));
        if (len > 0)
        {
            for (int i = 0; i < len; ++i)
            {
                if ('a' <= buf[i] && buf[i] <= 'z')
                {
                    buf[i] -= 'a';
                    buf[i] += 'A';
                }
                else if ('A' <= buf[i] && buf[i] <= 'Z')
                {
                    buf[i] -= 'A';
                    buf[i] += 'a';
                }
                else if ('0' <= buf[i] && buf[i] <= '9')
                {
                    buf[i] = '9' - (buf[i] - '0');
                }
                else
                {
                    printf("serial closed\n");
                    close(fd);
                    return 0;
                }
                write(fd, buf + i, 1);
                printf("write: %c\n", buf[i]);
            }
        }
    }
    
}
