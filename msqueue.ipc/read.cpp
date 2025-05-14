#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iostream>

using namespace std;
#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];
    ssize_t bytes_read;

    // Initialize the queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Open the message queue for reading
    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("Message queue opened for reading.\n");

    // Read messages from the queue
    while (1) {
        bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
        cout << "bytes_read: " << bytes_read << endl;
        if (bytes_read >= 0) {
            buffer[bytes_read] = '\0';
            printf("Received: %s\n", buffer);
            if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
                break;
            }
        } else {
            perror("mq_receive");
        }
    }

    // Cleanup
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}