// mmsg.h
#ifndef MMSG_H
#define MMSG_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128
typedef struct msgbuffer {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

#endif // MMSG_H