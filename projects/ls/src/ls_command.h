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
#include <memory>
#include <iostream>

using namespace std;


#define DO_NOT_IGNORE_DOT   0x1
#define NOT_LIST_DOT_FILE   0x2
#define COLORIZE_OUTPUT     0x4
#define LONG_LISTING        0x8


#define FLAG_a DO_NOT_IGNORE_DOT
#define FLAG_A NOT_LIST_DOT_FILE
#define FLAG_C COLORIZE_OUTPUT
#define FLAG_l LONG_LISTING


class ls_command {
    public:
        enum ReetCode {
            Success = 0,
            DeserializationError = 1,
            MemoryAllocationError = 2,
            OpenDirectoryError = 3,
            ParameterError = 4,
            SystemError = 5
        };


    public:
        static ls_command* get_instance(int argc, char* argv[]);

    protected:
        ls_command(int argc, char* argv[]);
        ReetCode deserialize_argc(int argc, char* argv[]);
        const string& get_help();

    protected:
        static ls_command* instance;
        string m_cur_dir = 0;
        uint64_t m_flags = 0;
};
