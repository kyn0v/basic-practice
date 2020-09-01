#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msgqueue.h"

void server(int, int);

int main(int argc, char** argv) {
    int msqid;
    //打开消息队列
    msqid = msgget(MQ_KEY, IPC_CREAT | 0666);
    if (msqid < 0) {
        fprintf(stderr, "Fail to open message queue: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    server(msqid, msqid);
    exit(EXIT_SUCCESS);
}

void server(int readfd, int writefd) {
    FILE* fp;
    pid_t clientPid;

    ssize_t recvBytes;
    msgbuf_t message;
    char* msgPtr;

    while (1) {
        // 接收消息(mtype = 1)
        recvBytes = msgrcv(readfd, &message, DATA_SIZE, 1, 0);  //阻塞读
        if (recvBytes <= 0) {
            //puts("No data received.");
            continue;
        }
        message.mdata[recvBytes] = '\0';
        // 分析消息: pid + 文件路径
        if ((msgPtr = strchr(message.mdata, ' ')) == NULL) {
            puts("Bogus request!");
            continue;
        }
        *msgPtr = '\0';
        msgPtr++;
        clientPid = atol(message.mdata);
        message.mtype = clientPid;
        fprintf(stderr, "Client(pid: %d) request file(%s).\n", clientPid, msgPtr);
        if ((fp = fopen(msgPtr, "r")) == NULL) {
            fprintf(stderr, "Can't open %s.\n", msgPtr);
            msgPtr--;
            *msgPtr = ' ';
            snprintf(message.mdata + recvBytes, sizeof(message.mdata) - recvBytes, ": can't open file!\n");
            if (msgsnd(writefd, &message, strlen(message.mdata), IPC_NOWAIT) == -1) {
                fprintf(stderr, "Fail to send message to client: %s.\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        } else {
            while (fgets(message.mdata, DATA_SIZE, fp) != NULL) {
                msgsnd(writefd, &message, strlen(message.mdata), IPC_NOWAIT);  //非阻塞写
            }
            puts("Send file success.");
        }
    }
}