/*
    Given a directory, write a program that will find all files with the same name in
    the directory and its sub directories. Show their name, which folder they are in
    and what day they were created.
    Expand the program to remove all duplicate
    copies based on user input. That is, ask user if each one of the file is to be kept or
    deleted. Based on user input, perform the appropriate action.
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>


#define BUFFER_SIZE 255
#define MAX_PATTERNS 20

enum ReetCode {
    SUCCESS = 0,
    ERROR_SYSTEM = -1,
    ERROR_INITIAL = -2
};

char* catagorize_patterns[MAX_PATTERNS];
char* catagorize_files[MAX_PATTERNS];
int num_files = 0;
char* directory = NULL;
const char* help = "Usage: sort_file -d <directory> -f <files> \
    \n\t-d <directory> : directory to parse \
    \n\t-f <files> : pattern to catagorize the tail's file \
";

int init() {
    for (int i = 0; i < MAX_PATTERNS; ++i) {
        catagorize_patterns[i] = (char*)malloc(BUFFER_SIZE * sizeof(char)); // Allocate memory
        if (catagorize_patterns[i] == NULL) {
            // Handle memory allocation failure
            fprintf(stderr, "Memory allocation failed for pattern %d\n", i);
            exit(ERROR_INITIAL);
        }

        catagorize_files[i] = (char*)malloc(BUFFER_SIZE * sizeof(char)); // Allocate memory
        if (catagorize_files[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for pattern %d\n", i);
            exit(ERROR_INITIAL);
        }
    }

    return SUCCESS;
}

void release() {
    for (int i = 0; i < MAX_PATTERNS; ++i) {
        if (catagorize_patterns[i]) {
            free(catagorize_patterns[i]);
            catagorize_patterns[i] = NULL;
        }
    }

    for (int i = 0; i < MAX_PATTERNS; ++i) {
        if (catagorize_files[i]) {
            free(catagorize_files[i]);
            catagorize_files[i] = NULL;
        }
    }

    if (directory) {
        free(directory);
        directory = NULL;
    }
}

int read_directory(char* direct) {
    if (direct == NULL) {
        fprintf(stderr, "Directory is not specified\n");
        exit(ERROR_INITIAL);
    }

    DIR* dir = opendir(direct);
    if (dir == NULL) {
        fprintf(stderr, "Open directory failed\n");
        return ERROR_SYSTEM;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            char* path = (char*)malloc(strlen(direct) + strlen(entry->d_name) + 2);
            if (path == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                return ERROR_SYSTEM;
            }

            sprintf(path, "%s/%s", direct, entry->d_name);
            read_directory(path);
            free(path);
        } else {
            for (int i = 0; i < num_files; ++i) {
                if (strstr(entry->d_name, catagorize_files[i]) != NULL) {
                    // printf("File: %s\n", entry->d_name);
                    // printf("Folder: %s\n", direct);
                    struct stat file_stat;
                    char* path = (char*)malloc(strlen(direct) + strlen(entry->d_name) + 2);
                    if (path == NULL) {
                        fprintf(stderr, "Memory allocation failed\n");
                        return ERROR_SYSTEM;
                    }

                    sprintf(path, "%s/%s", direct, entry->d_name);
                    if (stat(path, &file_stat) == -1) {
                        fprintf(stderr, "Get file stat failed\n");
                        return ERROR_SYSTEM;
                    }

                    // printf("Created date: %s\n", ctime(&file_stat.st_ctime));
                    fprintf(stdout, "%d-%d-%d %d:%d:%d %s %s\n", 1900 + localtime(&file_stat.st_ctime)->tm_year,
                            1 + localtime(&file_stat.st_ctime)->tm_mon, localtime(&file_stat.st_ctime)->tm_mday,
                            localtime(&file_stat.st_ctime)->tm_hour, localtime(&file_stat.st_ctime)->tm_min,
                            localtime(&file_stat.st_ctime)->tm_sec, direct, entry->d_name);
                    free(path);
                }
            }
        }
    }

    return SUCCESS;
}

/*
    Parsing argument from the command line
    command: sort_file -d <directory> -p <p>
    -d <directory> : directory to parse
    -f <files> : pattern to catagorize the tail's file
*/
// parsing the directory
int parse_arguments(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Invalid number of arguments\n");
        return ERROR_INITIAL;
    }

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'd') {
                if (i + 1 < argc) {
                    if (strncmp(argv[i + 1], ".",1) == 0 ) {
                        char cwd[BUFFER_SIZE];
                        if (getcwd(cwd, sizeof(cwd)) == NULL) {
                            fprintf(stderr, "Get current working directory failed\n");
                            exit(ERROR_INITIAL);
                        }
                        // parse the directory
                        directory = (char*)malloc(strlen(cwd) + 1);
                        memcpy(directory, cwd, strlen(cwd));
                        printf("Directory: %s\n", directory);
                    }
                    else {
                        // parse the directory
                        directory = (char*)malloc(strlen(argv[i + 1]) + 1);
                        if (directory == NULL) {
                            fprintf(stderr, "Memory allocation failed\n");
                            exit(ERROR_INITIAL);
                        }
                        memcpy(directory, argv[i + 1], strlen(argv[i + 1]));
                        printf("Directory: %s\n", directory);
                    }


                } else {
                    fprintf(stderr, "Invalid number of arguments\n %s", help);
                    exit(ERROR_INITIAL);
                }
            }
            else if (argv[i][1] == 'f') {
                for (int j = i + 1; j < argc; ++j) {
                    if (argv[j][0] != '-') {
                        strncpy(catagorize_files[num_files], argv[j], strlen(argv[j]));
                        printf("Type of files: %s\n", catagorize_files[num_files]);
                        num_files++;
                    } else {
                        fprintf(stderr, "Invalid number of arguments\n");
                        exit(ERROR_INITIAL);
                    }
                }
                // move index of argc to the next n arguments
                i += num_files;
            }
            else if (argv[i][1] == 'h') {
                printf("%s\n", help);
            }
        }
    }

    return SUCCESS;
}


// reading the directory
int main(int argc, char* argv[]) {
    char* error = (char*)malloc(BUFFER_SIZE);
    if (error == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return ERROR_INITIAL;
    }

    strcat(error, "command line error\n");
    strcat(error, help);

    if (init() != SUCCESS) {
        fprintf(stderr, "Initialization failed\n");
        release();
        free(error);
        return ERROR_INITIAL;
    }

    if (parse_arguments(argc, argv) != SUCCESS) {
        fprintf(stderr, "Parsing arguments failed\n");
        release();
        free(error);
        return ERROR_INITIAL;
    }

    if (read_directory(directory) != SUCCESS) {
        fprintf(stderr, "Reading directory failed\n");
        release();
        free(error);
        return ERROR_INITIAL;
    }
    release();

    free(error);

    return SUCCESS;
}
