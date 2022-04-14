#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
    /* receive from udp, write to uart */
    char *serial_name = "/dev/ttySIF0";
    // init udp

    int udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (udp < 0)
    {
        printf("failed to open udp socket: %s\n", strerror(errno));
        return 1;
    }
    else
    {
        printf("socket = %d\n", udp);
    }

    struct sockaddr_in sa = {.sin_family = AF_INET};
    if (bind(udp, (void *)&sa, sizeof sa) < 0)
    {
        printf("failed to bind: %s\n", strerror(errno));
        return 1;
    }

    socklen_t size = sizeof sa;
    if (getsockname(udp, (void *)&sa, &size) < 0)
    {
        printf("failed to getsockname: %s\n", strerror(errno));
        return 1;
    }

    printf("socket bind to %d\n", ntohs(sa.sin_port));

    // init uart

    int uart = open(serial_name, O_RDWR);

    if (uart < 0)
    {
        printf("failed to open %s: %s\n", serial_name, strerror(errno));
        return 1;
    }
    else
    {
        printf("uart = %d\n", udp);
    }

    // work

    char buf[1500] = "echo: ";
    while (1)
    {
        unsigned long len = read(udp, buf + 6, sizeof(buf) - 6);
        if (len == 1 && *buf == 'e')
        {
            printf("over.\n");
            break;
        }
        if (len > 0)
        {
            len += 6;
            buf[len] = '\0';
            printf("received: %s\n", buf + 6);
            write(uart, buf, len);
        }
    }
}
