#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFF_SIZE 1024

int parent_process(int shmid) {  // 写进程
    char *buf;
    void *shmaddr;
    sem_t *prsem, *pwsem;
    // 有名信号量
    if ((prsem = sem_open("rsem", O_CREAT, 0666, 0)) == SEM_FAILED) {
        perror("Fail to open rsem.");
        return -1;
    }
    if ((pwsem = sem_open("wsem", O_CREAT, 0666, 1)) == SEM_FAILED) {
        perror("Fail to open wsem.");
        return -1;
    }
    // 映射共享内存
    if ((shmaddr = shmat(shmid, NULL, 0)) == (void *)-1) {
        perror("Fail to shmat.");
        exit(EXIT_FAILURE);
    }
    buf = (char *)shmaddr;
    while (1) {
        if (sem_wait(pwsem) < 0) {
            perror("Fail to wait write sem.\n");
            break;
        }
        printf(">");
        fgets(buf, BUFF_SIZE, stdin);
        buf[strlen(buf) - 1] = '\0';
        if (sem_post(prsem) < 0) {
            perror("Fail to post read sem.\n");
            break;
        }
        if (strncmp(buf, "quit", 4) == 0) {
            if (shmdt(shmaddr) < 0) {
                perror("Fail to shmdt.");
                exit(EXIT_FAILURE);
            }
            break;
        }
        usleep(500);
    }
    return 0;
}

int child_process(int shmid) {  // 读进程
    char *buf;
    void *shmaddr;
    sem_t *prsem, *pwsem;
    // 有名信号量
    if ((prsem = sem_open("rsem", O_CREAT, 0666, 0)) == SEM_FAILED) {
        perror("Fail to open rsem.");
        return -1;
    }
    if ((pwsem = sem_open("wsem", O_CREAT, 0666, 1)) == SEM_FAILED) {
        perror("Fail to open wsem.");
        return -1;
    }
    // 映射共享内存
    if ((shmaddr = shmat(shmid, NULL, 0)) == (void *)-1) {
        perror("Fail to shmat.");
        exit(EXIT_FAILURE);
    }
    buf = (char *)shmaddr;
    while (1) {
        if (sem_wait(prsem) < 0) {
            perror("Fail to wait read sem.\n");
            break;
        }
        printf("read buf : %s.\n", buf);

        if (sem_post(pwsem) < 0) {
            perror("Fail to post read sem.\n");
            break;
        }
        if (strncmp(buf, "quit", 4) == 0) {
            if (shmdt(shmaddr) < 0) {
                perror("Fail to shmdt.");
                exit(EXIT_FAILURE);
            }
            break;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int shmid, pid;
    void *shmaddr;

    // 创建共享内存
    if ((shmid = shmget(IPC_PRIVATE, BUFF_SIZE, 0666 | IPC_CREAT)) < 0) {
        perror("Fail to shmget.");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) < 0) {
        perror("Fail to fork.");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        child_process(shmid);
    } else {
        parent_process(shmid);
        wait(NULL);
        if (shmctl(shmid, IPC_RMID, NULL) < 0) {
            perror("Fail to shmctl.");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}