#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define SIZE 1024

int main() {
   int fds[2]; 
   char w_buffer[SIZE] = "Writing message\n";
   
   fds[0] = open("r.txt", O_RDWR | O_EXCL);

    // writing message to stdout
    // this will print message to console
    write(1, w_buffer, SIZE);
    return 0;
}
