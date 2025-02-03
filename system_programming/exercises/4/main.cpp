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
                mkdir(catagorize_patterns[i], S_IRWXU | S_IRWXG | S_IROTH);
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
                    // do_move(src_file, des_file);
                    if (rename(src_file, des_file) == -1) {
                        perror("failed to rename");
                    }
                }
            }
        }
        get_dir = readdir(dir_fd);
    }
    closedir(dir_fd);

}

int main(int argc, char* argv[]) {
    setup();
    parse_catagorize(argc, argv);
    char current_path[256];
    categorize_files(getcwd(current_path, 256));
    release();
    return 0;
}