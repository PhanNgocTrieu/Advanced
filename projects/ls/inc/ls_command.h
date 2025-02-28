#ifndef _LS_COMMAND_H_
#define _LS_COMMAND_H_

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
#include "object.h"
#include "logger.h"

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
        static ls_command* get_instance();
        void init_with_argc(int argc, char* argv[]);
        void execute();



    protected:
        ls_command();
        virtual ~ls_command(){};
        ReetCode deserialize_argc(int argc, char* argv[]);
        const string& get_help();
        string get_cur_dir();
        void set_cur_dir(const string& dir);
        void set_flags(char c);


    protected:
        // Signals
        void emit_cur_dir_changed();

    protected:
        static ls_command* instance;
        Logger* m_logger = nullptr;
        string m_cur_dir = "";
        uint64_t m_flags = 0;
};

#endif
