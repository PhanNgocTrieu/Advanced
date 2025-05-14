#include "msg_queue.h"
#include <string.h>
#include <stdio.h>
MsgQueue::MsgQueue()
    : m_name(QUEUE_NAME), is_open(false), m_len(MAX_SIZE)
{
    // Initialize the queue attributes
    m_attr.mq_flags = 0;
    m_attr.mq_maxmsg = 10;
    m_attr.mq_msgsize = MAX_SIZE;
    m_attr.mq_curmsgs = 0;
}

MsgQueue::~MsgQueue() {
    is_open = false;
    mq_close(m_mq);
    mq_unlink(m_name);
}

void MsgQueue::send(const char* msg, size_t len) {
    if (!is_open || len > MAX_SIZE) {
        perror("mq_send out of range");
        return;
    }
    m_len = len;
    memset(m_buffer, 0, len);
    strncpy(m_buffer, msg, len);
    m_buffer[strcspn(m_buffer, "\n")] = '\0';

    if (mq_send(m_mq, m_buffer, len, 0) == -1) {
        perror("mq_send");
    }
    memset(m_buffer, 0, MAX_SIZE);
}

void MsgQueue::open(const char* name, int oflag) {
    if (strncmp(name, m_name, strlen(m_name)) != 0
    && is_open == true) {
        return;
    }

    m_mq = mq_open(name, oflag, 0777, &m_attr);
    if (m_mq == (mqd_t)-1) {
        perror("mq_open");
        return;
    }
}

void MsgQueue::setAttribute(long flag, long maxMsg, long maxMsgSize, long curMsgs) {
    m_attr.mq_flags = flag;
    m_attr.mq_maxmsg = maxMsg;
    m_attr.mq_msgsize = maxMsgSize > MAX_SIZE ? MAX_SIZE : maxMsgSize;
    m_attr.mq_curmsgs = curMsgs;

    if (is_open) {
        mq_setattr(m_mq, &m_attr, NULL);
    }
}

ssize_t MsgQueue::receive() {
    ssize_t bytes_read = mq_receive(m_mq, m_buffer, m_len, NULL);
    if (bytes_read >= 0) {
        m_buffer[bytes_read] = '\0';
        fprintf(stdout, "Received: %s\n", m_buffer);
    } else {
        perror("mq_receive");
    }
    return bytes_read;
}