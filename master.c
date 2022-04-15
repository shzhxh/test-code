/* running on master computer, talk with test computer, and record time */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(){
    const int LEN = 256;
    int udp_fd; // udp fd
    int ret;    // return value
    int loops = 10;
    struct sockaddr_in s_addr; 
    char buf[LEN];  
    char* server_ip = "192.168.0.218";

    // init udp
    udp_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_fd < 0){
        printf("socket creating error.");
        exit(1);
    }
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(1234);
    s_addr.sin_addr.s_addr = inet_addr(server_ip);
    
    for(;loops >= 0; loops--){
        ret = sendto(udp_fd, "a", 1, 0, (struct sockaddr*) &s_addr, sizeof(struct sockaddr));
        if (ret < 0){
            printf("sendto fail : %s\n", strerror(errno));
            return 1;
        }
        recv(udp_fd, buf, LEN, 0);
        printf("recv : %s\n", buf);
    }
    ret = sendto(udp_fd, "e", 1, 0, (struct sockaddr*) &s_addr, sizeof(struct sockaddr));

    close(udp_fd);
}