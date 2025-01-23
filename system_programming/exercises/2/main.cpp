/*
    Write a program to read all txt files (that is files that ends with .txt) in the current
    directory and merge them all to one txt file and returns a file descriptor for the
    new file.
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

/*
    Reading current directory to find the txt file
    Reading content fron txt files
*/

DIR* dir_fd;
std::vector<char*> doc_files;
std::vector<int> fds;
char doc_name[5] = ".txt";
char out_file[] = "out.txt";
int out_fd;
bool init = false;

void init_files() {
    if ( creat(out_file, S_IREAD | S_IWRITE) == -1 ) {
        perror("create file failed");
        return;
    }
    dir_fd = opendir(DEFAULT_DIR);
    init = true;
}

int getting_files() {
    if (!init) return ERROR_INITIAL;

    struct dirent* entry = readdir(dir_fd);
    while (entry != nullptr) {
        printf("files: %s\n", entry->d_name);
        // Create pointer and move to the *.txt position
        char* f_ptr = &entry->d_name[0];
        f_ptr = f_ptr + (strlen(entry->d_name) - 4);
        if ( strncmp(f_ptr, doc_name, 4) == 0 ) {
            doc_files.push_back(entry->d_name);
        }
        entry = readdir(dir_fd);
    }

    return SUCCESS;
}

int read_files() {
    if (!init) return ERROR_INITIAL;

    out_fd = open(out_file, O_CREAT | O_RDWR);

    if (out_fd == ERROR_SYSTEM) {
        perror("Could not open file");
        return ERROR_SYSTEM;
    }

    if (doc_files.empty()) {
        puts(".txt files is empty");
        return ERROR_SYSTEM;
    }

    // Reading content from files
    for (auto f : doc_files) {
        char file_path[255] = "docs/";
        strcat(file_path, f);
        printf("handle file: %s\n", file_path);

        int fd = open(file_path, O_RDONLY);
        if (fd == ERROR_SYSTEM) {
            printf("could not open %s\n", f);
            continue;
        }

        char buffer[BUFFER_SIZE];
        memset(buffer,'\0', BUFFER_SIZE);
        size_t s = read(fd, buffer, BUFFER_SIZE);

        while( s > 0 ) {
            printf("buffer: %s\n", buffer);
            write(out_fd, buffer, s);
            // move to s bytes
            lseek(fd, s, 1);
            s = read(fd, buffer, BUFFER_SIZE);
        }
        write(out_fd,"\n",1);
        close(fd);
    }

    return out_fd;
}

void release() {
    close(out_fd);
    closedir(dir_fd);
}

// finding file in dir
int search_file(char* file, DIR* dir = nullptr) {
    if (!init) return ERROR_INITIAL;

    DIR* internal_dir = dir == nullptr ? dir_fd : dir;

    if (internal_dir == nullptr) {
        return ERROR_SYSTEM;
    }


    struct dirent* entry = readdir(internal_dir);
    while (entry != NULL) {
        if (
           strlen(entry->d_name) == strlen(file) && (strcmp(entry->d_name, file) == 0)
        ) {
            return SUCCESS; /* return success */
        }
        entry = readdir(internal_dir);
    }

    // release internal pointer
    internal_dir = nullptr;

    return ERROR_SYSTEM;
}


int main() {
    init_files();
    getting_files();
    read_files();
    release();
    return 0;
}