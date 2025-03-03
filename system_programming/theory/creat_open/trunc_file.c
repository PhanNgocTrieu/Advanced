#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>

#define SIZE 4086
#define FILE_NAME "r.txt"
int main() {
    int fd = open(FILE_NAME, O_TRUNC | O_EXCL | O_RDWR);

    if (fd < 0) {
        perror("open file");
    }

    char buffer[SIZE];
    memset(buffer,'1',SIZE);
    buffer[SIZE-1]='\n';
    
    int bytes = 0;
    while ( (bytes = write(fd, buffer, SIZE)) > 0) {
        fprintf(stdout, "wrote %d bytes to file %s\n", bytes, FILE_NAME);
        sleep(1);
    }

    close(fd);

    return 0;
}