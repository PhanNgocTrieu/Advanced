/*
    Given a directory, write a program that will find all files with the same name in
    the directory and its sub directories. Show their name, which folder they are in
    and what day they were created. Expand the program to remove all duplicate
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
#include <iostream>
#include <time.h>
#include <queue>
#include <unordered_map>


#define BUFFER_SIZE 255
#define MAX_PATTERNS 20

enum ReetCode {
    SUCCESS = 0,
    ERROR_SYSTEM = -1,
    ERROR_INITIAL = -2
};


std::queue<char*> directories;
std::vector<char*> file_tails;
char* catagorize_patterns[MAX_PATTERNS];
std::unordered_map<std::string, std::string> files_as_tail;
int num_files = 0;
char* directory = nullptr;
const char* help = "Usage: sort_file -d <directory> -f <files> \
    \n\t-d <directory> : directory to parse \
    \n\t-f <files> : pattern to catagorize the tail's file \
";

void init() {
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

    if (directory) {
        delete directory;
        directory = nullptr;
    }
}

void read_directory(char* direct) {
    if (direct == nullptr) {
        throw std::runtime_error("Directory is not specified");
    }

    directories.push(direct);

    while (!directories.empty()) {
        char* _dir = directories.front();
        directories.pop();

        DIR* dir;
        struct dirent* ent;
        if ((dir = opendir(_dir)) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                printf("open dir: %s\n", ent->d_name);
                if (ent->d_type == DT_DIR) {
                    if (ent->d_name != "." && ent->d_name != "..") {
                        printf("pushing dir: %s\n", ent->d_name);
                        directories.push(ent->d_name);
                    }
                } else if (ent->d_type == DT_REG) {
                    printf("files: %s\n", ent->d_name);
                }
            }
            closedir(dir);
        } else {
            perror("Error: ");
        }
    }
}

/*
    Parsing argument from the command line
    command: sort_file -d <directory> -p <p>
    -d <directory> : directory to parse
    -f <files> : pattern to catagorize the tail's file
*/
// parsing the directory
void parse_arguments(int argc, char* argv[]) {

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'd') {
                if (i + 1 < argc) {
                    // parse the directory
                    directory = new char[strlen(argv[i + 1]) + 1];
                    memcpy(directory, argv[i + 1], strlen(argv[i + 1]));
                    printf("Directory: %s\n", directory);
                } else {
                    throw std::runtime_error("Invalid number of arguments");
                }
            }
            else if (argv[i][1] == 'f') {
                for (int j = i + 1; j < argc; ++j) {
                    if (j < argc) {
                        catagorize_patterns[num_files] = new char[strlen(argv[j]) + 1];
                        strcpy(catagorize_patterns[num_files], argv[j]);
                        printf("Pattern: %s\n", catagorize_patterns[num_files]);
                        num_files++;
                    } else {
                        throw std::runtime_error("Invalid number of arguments");
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
}


// reading the directory
int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            char* error = new char[BUFFER_SIZE];
            strcat(error, "command line error\n");
            strcat(error, help);
            throw std::runtime_error(error);
        }

        init();
        parse_arguments(argc, argv);
        read_directory(directory);
        release();
    } catch (const std::exception& e) {
        printf("Error: %s\n", e.what());
        return ERROR_INITIAL;
    }

    return SUCCESS;
}
