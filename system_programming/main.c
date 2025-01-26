#include <stdio.h>
#include <sys/file.h> // can be replace <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h> // or #include <unistd.h>
#include <unistd.h>
#include <string.h>

char FILE_NAME[255] = "doc.txt";
char MESSAGE_TO_WIRTE[1024] = "\nHello every one! My name is John. I am 25 years old! This is added";

int create_file(char* file, int mode) {
    /*
        #include <sys/stat.h>
        S_IREAD --- read permission for the owner
        S_IWRITE --- write permission for the owner
        S_IEXEC --- execute/search permission for the owner
        S_IRWXU --- read, write, execute permission for the user
        S_IRGRP – read for group
        S_IWGRP – write for group
        S_IXGRP – execute for` group
        S_IRWXG – read, write, execute for the group
        S_IROTH --- read for others
        S_IWOTH – write for others
        S_IXOTH -- execute for others
        S_IRWXO – read , write , execute for others
    */
    return creat(file, mode);
}


int open_file(char* file, int flag, int mode) {
    /*
        open(arguments) {}
        arguments:
            filename : A string that represents the absolute, relative or filename of the file
            flags : An integer code describing the access (see below for details)
            mode : The file protection mode usually given by 9 bits indicating rwx permission

        flags:
            O_RDONLY -- opens file for read only
            O_WRONLY – opens file for write only
            O_RDWR – opens file for reading and writing
            O_NDELAY – prevents possible blocking
            O_APPEND --- opens the file for appending
            O_CREAT -- creates the file if it does not exists
            O_TRUNC -- truncates the size to zero
            O_EXCL – produces an error if the O_CREAT bit is on and file exists

        For opening files, the third argument can always be left at 0.
    */
    return open(file, flag, mode);
}


size_t read_file(int _fd, char* _buffer, size_t _bytes) {
    /*
        fd is the file descriptor
        buffer is address of a memory area into which the data is read
        bytes is the maximum amount of data to read from the stream
            Bytes should not exceed the size of the buffer

        return bytes of writing, 0 if reading nothing
    */
    return read(_fd,_buffer,_bytes);
}

size_t write_file(int _fd, char* _buffer, size_t _bytes) {
    /*
        fd is the file descriptor
        buffer is address of a memory area into which the data is read
        bytes is the maximum amount of data to read from the stream
            Bytes should not exceed the size of the buffer
    */
    return write(_fd, _buffer, _bytes);
}

int readline(char s[], int size) {
    char * tmp = s;
    while(--size > 0 && read(0, tmp, 1) != 0 && *tmp++ != '\n') {
        *tmp = '\0';
    }
    return (tmp-s);
}

void _close(int _fd) {
    close(_fd);
}


int main(int argc, char argv[]) {
    char* word = "hello every one";
    printf("size of word: %ld\n", strlen(word));

    if (create_file(FILE_NAME, S_IREAD | S_IWRITE) == -1) {
        perror("create file failed");
        return -1;
    }

    int fd = open_file(FILE_NAME, O_RDWR, 0);
    if (fd == -1) {
        perror("Read file failed");
        _close(fd);
        return -1;
    }

    printf("Open file successful: %d\n", fd);

    char buffer[1024];
    memset(buffer,'\0', 1024);
    size_t bytes = read_file(fd, buffer, 1024);
    if (bytes < 0) {
        perror("reading file fails");
        _close(fd);
        return -1;
    }
    printf("reading content: %ld bytes - %s\n", bytes, buffer);

    bytes = write_file(fd, MESSAGE_TO_WIRTE, strlen(MESSAGE_TO_WIRTE) + 1);
    if (bytes != strlen(MESSAGE_TO_WIRTE) + 1) {
        perror("writing went wrong!");
        _close(fd);
        return -1;
    }

    _close(fd);
    return 0;
}