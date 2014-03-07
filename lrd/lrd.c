#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdlib.h>

char const * const arduino_port = "/dev/ttyACM0";

int open_arduino() {
    int arduino = open("/dev/ttyACM0", O_RDWR | O_NDELAY | O_NOCTTY);
    if (arduino == -1) {
        char err[1000] = {0};
        strncat(err, "Could not find arduino at ", 1000);
        strncat(err, arduino_port, 1000);
        perror(err);
        exit(-1);
    }
    fcntl(arduino, F_SETFL, 0);
    return arduino;
}

int close_arduino(int arduino) {
    close(arduino);
}

int main(void) {
    int socket_fd, n;
    socklen_t len;
    char line[128];
    struct sockaddr_in saddr, caddr;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(2910);

    bind(socket_fd, (struct sockaddr *)&saddr, sizeof(saddr));

    printf("Server running\n");
    while(1) {
        len = sizeof(caddr);
        n = recvfrom(socket_fd, line, 128, 0, (struct sockaddr *)&caddr, &len);
        sendto(socket_fd, line, n, 0, (struct sockaddr *)&caddr, len);
        line[n] = 0;
        fputs(line, stdout);
    }

    return 0;
}