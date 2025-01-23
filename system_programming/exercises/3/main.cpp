/*
    Write a program that takes a string as an argument and return all the files that
    begins with that name in the current directory and its sub directories. For example
    > ./a.out foo will return all file names that begins with foo.
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
#include <memory>

#define MAX_PARAMS 10
#define SUCCESS 0
#define FAILED -1
#define ERROR_INITAL -2
#define ERROR_ARGUMENT -3
#define SIZE_BUFFER 255

#define CURRENT_DIR "."

char* input_params[MAX_PARAMS];

int params_parsing(int argc, char* argv[]) {
    if (argc < 2) {
        return ERROR_ARGUMENT;
    }
    // Initialize as nullptr
    for (int i = 0; i < MAX_PARAMS; ++i) {
        input_params[i] = nullptr;
    }
    // Parsing argument
    for (int i = 0; i < argc; ++i) {
        // allocated memory
        input_params[i] = new char[SIZE_BUFFER];
        // initializing value
        memset(input_params[i],'\0',SIZE_BUFFER);
        // parsing value from argv
        strncpy(input_params[i], argv[i], strlen(argv[i]));
    }
    return SUCCESS;
}

void trace_params_mapping() {
    for (int i = 0; i < MAX_PARAMS; ++i) {
        if (input_params[i] != nullptr)
            printf("params %d -> %s\n", i, input_params[i]);
    }
}

enum PARAM_TYPE {
    DIRECTORY = 1,
    OPTION,
    REGEX
};

char* get_params(const PARAM_TYPE& T) {
    return input_params[(int)T];
}

void do_list_files(const char* dir, const char* regex, bool isSub = false) {
    if (dir == nullptr || regex == nullptr) {
        return;
    }

    if (isSub && strncmp(dir, ".", 1) == 0) { return; }

    DIR* dir_fd = opendir(dir);
    if (dir_fd == nullptr) {
        // fprintf(stdout, "open directory %s failed\n", dir);
        return;
    }

    dirent* dir_entry = readdir(dir_fd);
    while (dir_entry) {
        // printf("current file: %s\n", dir_entry->d_name);
        do_list_files(dir_entry->d_name, regex, true);

        if (strncmp(dir_entry->d_name, regex, strlen(regex)) == 0) {
            printf("%s\n", dir_entry->d_name);
        }
        // move to next file
        dir_entry = readdir(dir_fd);
    }

    closedir(dir_fd);
}

void release() {
    for (int i = 0; i < MAX_PARAMS; ++i) {
        if (input_params[i]) {
            delete input_params[i];
            input_params[i] = nullptr;
        }
    }
}

int main(int argc, char* argv[]) {
    if (params_parsing(argc, argv) == ERROR_ARGUMENT) {
        puts("Error in parsing value");
        return 0;
    }

    auto get_directory = get_params(PARAM_TYPE::DIRECTORY);
    auto get_regex = get_params(PARAM_TYPE::REGEX);
    // printf("get_directory: %s\n", get_directory);
    // printf("get_regex: %s\n", get_regex);

    do_list_files(get_directory, get_regex);

    release();

    return 0;
}