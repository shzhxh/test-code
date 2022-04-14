#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int fd;
    struct sockaddr_in sa;
    
	/* read from udp */
    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (fd < 0)
    {
        printf("failed to open socket: %s\n", strerror(errno));
        return 1;
    }

    printf("socket = %d\n", fd);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(1234);
    if (bind(fd, (void *)&sa, sizeof sa) < 0)
    {
        printf("failed to bind: %s\n", strerror(errno));
        return 1;
    }

    printf("bind done\n");

    socklen_t size = sizeof sa;
    if (getsockname(fd, (void *)&sa, &size) < 0)
    {
        printf("failed to getsockname: %s\n", strerror(errno));
        return 1;
    }

    printf("socket bind to %d\n", ntohs(sa.sin_port));

    char buf[1500];
    while (1)
    {
        unsigned long len = read(fd, buf, sizeof buf);
        if (len == 1 && *buf == 'e')
        {
            printf("over.\n");
            break;
        }
        else if (len > 0)
        {
            buf[len] = '\0';
            printf("received: %s\n", buf);
        }
    }
}
