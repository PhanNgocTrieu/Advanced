#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <mqueue.h>
#define QUEUE_NAME "/queue_comm"
#define MAX_SIZE 1024
#define MSG_STOP "exit"

class MsgQueue {
    public:
        MsgQueue();
        ~MsgQueue();
        void setAttribute(long flag, long maxMsg, long maxMsgSize, long curMsgs);
        void send(const char* msg, size_t len);
        ssize_t receive();
        void open(const char* name, int oflag);
    private:


    protected:
        mqd_t m_mq;
        mq_attr m_attr;
        char m_buffer[MAX_SIZE];
        const char* m_name;
        int m_len;
        bool is_open;
};

#endif // MSG_QUEUE_H