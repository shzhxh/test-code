/* running on master computer, talk with slave computer, and record time */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define LEN 256
char buf[LEN];
int loops = 10;
struct timeval time_start, time_end;
double sum; // total time

void net_test(char* slave_ip){
    int udp_fd; // udp fd
    struct sockaddr_in s_addr; 

    // init udp
    udp_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_fd < 0){
        printf("socket creating error.");
        exit(1);
    }
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(1234);
    s_addr.sin_addr.s_addr = inet_addr(slave_ip);
    
    gettimeofday(&time_start, NULL);
    for(int i = 0; i <= loops; i++){
        sendto(udp_fd, "a", 1, 0, (struct sockaddr*) &s_addr, sizeof(struct sockaddr));
        recv(udp_fd, buf, LEN, 0);
        printf("recv : %s\n", buf);
    }
    gettimeofday(&time_end, NULL);
    sendto(udp_fd, "e", 1, 0, (struct sockaddr*) &s_addr, sizeof(struct sockaddr));

    close(udp_fd);
}
void serial_test(char* slave_serial){
    int serial_fd;

    serial_fd = open(slave_serial, O_RDWR);
    if(serial_fd < 0){
        fprintf(stderr, "failed to open %s: %d\n", slave_serial, serial_fd);
        return -1;
    }

    gettimeofday(&time_start, NULL);
    for(int i = 0; i <= loops; i++){
        write(serial_fd, "a", 1);
        if(read(serial_fd, buf, LEN) > 0) printf("read: %s", buf);
    }
    gettimeofday(&time_end, NULL);

    close(serial_fd);
}

void print_result(void){
    sum = (time_end.tv_sec - time_start.tv_sec) * 1000000;
    printf("total sec : %f\n", sum);
    sum += (time_end.tv_usec - time_start.tv_usec);
    printf("total usec : %f\n", sum);
    printf("avg time(us) : %f\n", sum/loops); 
}

void usage(void){
    fprintf(stderr, "Usage :\n\
      master -n(et) $slave_ip\n\
      master -s(erial) $serial_dev\n\
      master -p(ingpong) \n");
    exit(1);
}

int isoption(char* option, char* test){
    if(strcmp(option, test) == 0) return 1;
    if(option[0] != '-' && strlen(option) != 2) return 0;
    if(option[1] == test[1]) return 1;
    return 0;
}

int main(int argc, char* argv[]){
    if(argc != 3) usage();
    if(isoption(argv[1], "-net")) net_test(argv[2]);
    if(isoption(argv[1], "-serial")) serial_test(argv[2]);

    print_result();
}
