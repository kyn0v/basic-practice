#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX 100

int child_read_pipe(int pfd, char *fname) {
    char buf[MAX];
    int ret, fd;
    if ((fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
        fprintf(stderr, "Fail to open %s : %s.\n", fname, strerror(errno));
        return -1;
    }
    while (ret = read(pfd, buf, sizeof(buf))) {
        write(fd, buf, ret);
    }
    close(pfd);
    return 0;
}

int parent_write_pipe(int pfd, char *fname) {
    char buf[MAX];
    int ret, fd;
    if ((fd = open(fname, O_RDONLY)) < 0) {
        fprintf(stderr, "Fail to open %s : %s.\n", fname, strerror(errno));
        return -1;
    }
    while (ret = read(fd, buf, sizeof(buf))) {
        write(pfd, buf, ret);
    }
    close(pfd);
    return 0;
}

int main(int argc, char *argv[]) {
    int pid;
    int fd[2];
    if(argc<3){
        fprintf(stderr,"usage %s argv[1] argv[2].\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    if (pipe(fd) < 0) {
        perror("Fail to pipe.\n");
        exit(EXIT_FAILURE);
    }
    if((pid=fork())<0){
        perror("Fail to fork.");
        exit(EXIT_FAILURE);
    } else if(pid > 0) {    // Parent process
        close(fd[0]);
        parent_write_pipe(fd[1], argv[1]);  //src
        wait(NULL);
    } else{ // Child process
        close(fd[1]);
        child_read_pipe(fd[0], argv[2]);    //des
    }
    exit(EXIT_SUCCESS);
    // return 0;
}