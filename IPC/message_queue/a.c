#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "msgqueue.h"

void client(int, int);

int main(int argc, char** argv) {
    int msqid;
    //打开消息队列
    msqid = msgget(MQ_KEY, IPC_CREAT | 0666);
    if (msqid < 0) {
        fprintf(stderr, "Fail to open message queue: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    client(msqid, msqid);
    exit(EXIT_SUCCESS);
}

void client(int readfd, int writefd) {
    // 构造消息：消息类型 + 进程id + " " + 消息内容(客户端请求的文件路径)
    msgbuf_t message;
    while (1) {
        message.mtype = 1;
        long pid = getpid();
        snprintf(message.mdata, DATA_SIZE, "%ld ", pid);
        ssize_t offset = strlen(message.mdata);
        printf(">");
        char* filePath = message.mdata + offset;
        fgets(filePath, DATA_SIZE - offset, stdin);
        offset = strlen(message.mdata);
        if (message.mdata[offset - 1] == '\n')  //删除换行符
        {
            message.mdata[offset - 1] = '\0';
        }
        // 发送消息(mtype = 1)
        if (msgsnd(writefd, &message, strlen(message.mdata), 0) == -1) {
            fprintf(stderr, "Fail to send message to server: %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        // 接收消息(mtype = pid)
        int recvBytes;
        if((recvBytes = msgrcv(readfd, &message, DATA_SIZE, pid, 0)) > 0) {
            write(STDOUT_FILENO, message.mdata, recvBytes);
        }
    }
}