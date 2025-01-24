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
#define MAX_PATTERNS 20
#define CURRENT_DIRECTORY "."

char* catagorize_patterns[MAX_PATTERNS];
char** file_holders[MAX_PATTERNS];
int g_argc = 0;

void setup() {
    for (int i = 0; i < MAX_PATTERNS; ++i) {
        catagorize_patterns[i] = nullptr;
    }
}

void release() {
    for (int i = 0; i < MAX_PATTERNS; ++i) {
        if (catagorize_patterns[i]) {
            delete catagorize_patterns[i];
            catagorize_patterns[i] = nullptr;
        }
    }
}


void create_directory() {
    for (int i = 0; i < MAX_PATTERNS; ++i) {
        if (catagorize_patterns[i] != nullptr) {
            if (access(catagorize_patterns[i], R_OK | W_OK) == -1) {
                mkdir(catagorize_patterns[i], 0444);
            }
        }
    }
}

void parse_catagorize(int argc, char* argv[]) {
    int s = argc > 20 ? MAX_PATTERNS : argc;
    g_argc = s - 1;
    printf("s: %d\n", s);
    for (int i = 1; i < s; ++i) {
        printf("arg[%d]: %s\n", i, argv[i]);
        catagorize_patterns[i - 1] = new char[BUFFER_SIZE];
        memset(catagorize_patterns[i - 1],'\0', BUFFER_SIZE);
        memcpy(catagorize_patterns[i - 1], argv[i], strlen(argv[i]));
    }

    create_directory();
}

void do_move(const char* src, const char* des) {
    if (access(des, F_OK) == -1) {
        if (creat(des, S_IREAD | S_IWRITE) == -1) {
            printf("create file %s failed\n", des);
            return;
        }
        printf("create file: %s\n", des);
    }

    int d_fd = open(des, O_RDWR | O_CREAT);
    int s_fd = open(src, O_RDONLY);
    if (d_fd == -1 || s_fd) {
        perror("Could no open file to read/write");
        return;
    }

    char buffer[1024];
    size_t bytes = read(s_fd, buffer, 1024);
    while (bytes > 0)
    {
        write(d_fd, buffer, bytes);

        // move position to last
        lseek(d_fd, bytes, 1);
        lseek(s_fd, bytes, 1);

        // continue to read;
        bytes = read(s_fd, buffer, 1024);
    }

    // remove file
    remove(src);

    close(s_fd);
    close(d_fd);
}

void categorize_files(const char* f_path) {
    DIR* dir_fd = opendir(f_path);
    if (dir_fd == NULL) {
        return;
    }

    dirent* get_dir = readdir(dir_fd);
    while (get_dir)
    {
        // We can optimize this with map in c++ to check condition
        if (strlen(get_dir->d_name) >= 4) {
            for (int i = 0; i < g_argc; ++i) {
                char f_tail[4];
                strncpy(f_tail, (get_dir->d_name) + (strlen(get_dir->d_name) - 3), 3);
                // printf("file's tail: %s - %s\n", f_tail, catagorize_patterns[i]);

                if (strncmp(f_tail, catagorize_patterns[i], 3) == 0) {
                    char current_path[256];
                    getcwd(current_path, 256);
                    // create source file
                    char* src_file = new char[BUFFER_SIZE];
                    memcpy(src_file, current_path, strlen(current_path));
                    strcat(src_file, "/");
                    strcat(src_file, get_dir->d_name);
                    printf("src: %s\n", src_file);

                    // create destination file
                    char* des_file = new char[BUFFER_SIZE];
                    memcpy(des_file, current_path, strlen(current_path));
                    strcat(des_file, "/");
                    strcat(des_file, catagorize_patterns[i]);
                    strcat(des_file, "/");
                    strcat(des_file, get_dir->d_name);
                    printf("des: %s\n", des_file);
                    do_move(src_file, des_file);
                }
            }
        }
        get_dir = readdir(dir_fd);
    }
    closedir(dir_fd);

}

int main(int argc, char* argv[]) {
    if (creat("file.bmp", S_IREAD | S_IWRITE) == -1) {
        printf("create file %s failed\n", "bmp/file.bmp");
        return -1;
    }
    printf("create file: %s\n", "bmp/file.bmp");
    // setup();
    // parse_catagorize(argc, argv);
    // char current_path[256];
    // getcwd(current_path, 256);
    // // printf("current path: %s\n", current_path);
    // categorize_files(current_path);
    // release();
    return 0;
}