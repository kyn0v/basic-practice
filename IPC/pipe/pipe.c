// task: 父进程向管道中写数据，子进程从管道中读取数据

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 10
#define MAX 100

int child_read_pipe(int fd) {
    char buf[N];
    int ret = 0;
    while (1) {
        ret = read(fd, buf, sizeof(buf));
        if (ret > 0) {
            buf[ret] = '\0';
            printf("Read %d byte: %s.\n", ret, buf);
            if (strncmp(buf, "quit", 4) == 0)
                break;
        } else if (ret == 0) {
            printf("No data in pipe written by Parent process.\n");
        } else {
            perror("Read error.");
            break;
        }
    }
    close(fd);
    return 0;
}

int parent_write_pipe(int fd) {
    char buf[MAX] = {0};
    int ret = 0;
    while (1) {
        printf(">");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = '\0';
        ret = write(fd, buf, strlen(buf));
        if(ret==-1){
            perror("Write error.");
            break;
        }
        usleep(500);
        if (strncmp(buf, "quit", 4) == 0) {
            break;
        }
    }
    close(fd);
    return 0;
}

int main() {
    int fd[2];
    if (pipe(fd) < 0) {
        perror("Fail to pipe");
        exit(EXIT_FAILURE);
    }
    int pid, ppid;
    int ret = fork();
    if (ret > 0) {
        pid = getpid();
        ppid = getppid();
        printf("This is Parent process: pid = %d, ppid = %d, and it forks Child process: pid = %d\n", pid, ppid, ret);
        close(fd[0]);
        parent_write_pipe(fd[1]);
    } else if (ret == 0) {
        pid = getppid();
        ppid = getppid();
        printf("This is Child process: pid %d, ppid %d\n", pid, ppid);
        close(fd[1]);
        child_read_pipe(fd[0]);
    } else {  // ret == -1
        perror("Fail to fork");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
    // return 0;
}