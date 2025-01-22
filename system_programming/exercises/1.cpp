/*
    https://www.cs.cmu.edu/~guna/15-123S11/Lectures/Lecture24.pdf

    Write a function foo(int fd, char* buf, int b_size, int n, int skip) that reads to buf
    from file with file descriptor fd, n blocks of size b_size each. The last argument
    specifies how many bytes to skip after reading each block. Return -1 if the
    operation is unsuccessful. Else return total number of bytes read.
*/
#include <sys/fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <string.h>

char file_1[255] = "1.txt";
char file_2[255] = "1_2.txt";

size_t foo(int fd, char* buff, int b_size, int n, int skip) {
    size_t total = 0;
    size_t bytes = 0;
    char _buf[b_size];
    while(n--) {
        if ( (bytes = read(fd, _buf, b_size)) == -1) {
            perror("reading error!");
            bytes = 1;
            break;
        }

        if (bytes == 0) {
            puts("End of the file already");
            break;
        }
        strcat(buff, _buf);
        total += bytes;
        // set fd pointer to skip position
        lseek(fd, skip, 1);
    }
    return bytes;
}

int main(int argc, char argv[]) {
    int fd;
    if ((fd = open(file_2, O_RDONLY)) == -1) {
        perror("open file failed");
        return -1;
    }

    char read_buff[1024];
    memset(read_buff, '\0', 1024);
    size_t _b = foo(fd, read_buff, 12, 3, 4);

    if (_b == -1) {
        perror("reading error!");
        close(fd);
        return -1;
    }
    else if (_b == 0) {
        puts("file is empty! content is nothing");
    }
    else {
        printf("read: %s\n", read_buff);
    }
    return 0;
}