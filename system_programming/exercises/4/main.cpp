/*
    Write a program that will categorize all files in the current folder based on their
    file type. That is all .txt file in one folder called txt, all .bmp files in another folder
    called bmp etc. The argument to the program is a folder name.
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <vector>
#include <dirent.h>
#include <sys/dir.h>
#include <string.h>

#define BUFFER_SIZE 255
#define SUCCESS 0
#define ERROR_SYSTEM -1
#define ERROR_INITIAL -2
#define DEFAULT_DIR "docs"

int main(int argc, char* argv[]) {
    
    return 0;
}