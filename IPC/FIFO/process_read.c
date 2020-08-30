#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 65535

int main(int argc, char *argv[]) {
    int fd, n;
    char buf[MAX];
    if (argc < 2) {
        fprintf(stderr, "usage: %s argv[1].\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (mkfifo(argv[1], 0666) < 0 && errno != EEXIST) {
        fprintf(stderr, "Fail to mkfifo %s : %s.\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr, "Fail to open %s : %s.\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("Open FIFO for read, success.\n");
    while (1) {
        printf(">");
        scanf("%s", buf);
        if (strncmp("quit", buf, 4) == 0) {
            break;
        }
        n = atoi(buf);
        n = read(fd, buf, n);
        buf[n] = '\0';
        printf("Read %d bytes: %s.\n", n, buf);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}