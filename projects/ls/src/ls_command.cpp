#include "ls_command.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

using namespace std;

ls_command* ls_command::instance = nullptr;

ls_command* ls_command::get_instance() {
    if (instance == nullptr) {
        instance = new ls_command();
    }
    return instance;
}

ls_command::ls_command() : m_flags(0) {
    m_logger = Logger::get_instance("ls");
    m_logger->log(Logger::LogLevel::Info, "ls_command instance created");
}

ls_command::ReetCode ls_command::deserialize_argc(int argc, char* argv[]) {
    // traverse the arguments
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == nullptr) {
            return ReetCode::ParameterError;
        }
        // detecting the flag
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'h') {
                // print the help
                cout << get_help() << endl;
                return ReetCode::Success;
            }
            // using multiple flags
            for (int j = 1; j < strlen(argv[i]); ++j) {
                set_flags(argv[i][j]);
            }
        } else {
            // detecting the directory
            string cur_dir = string(argv[i]);
            if (cur_dir == ".") {
                cur_dir = get_cur_dir();
            }
            set_cur_dir(cur_dir);
        }
    }

    return ls_command::ReetCode::Success;
}

void ls_command::init_with_argc(int argc, char* argv[]) {
    m_logger->log(Logger::LogLevel::Info, "Initializing ls with argc");
    deserialize_argc(argc, argv);
}

void ls_command::execute() {
    // Implementation of the execute method
    m_logger->log(Logger::LogLevel::Info, "Executing ls");
}

void ls_command::set_cur_dir(const string& dir) {
    if (m_cur_dir != dir) {
        m_cur_dir = dir;
        emit_cur_dir_changed();
    }
}

void ls_command::set_flags(char c) {
    switch (c) {
        case 'a': {
            // detecting the dot file
            m_flags |= FLAG_a;
            break;
        }
        case 'A': {
            // detecting the dot file
            m_flags |= FLAG_A;
            break;
        }
        case 'l': {
            // detecting the long listing
            m_flags |= FLAG_l;
            break;
        }
        case 'C': {
            // detecting the colorize output
            m_flags |= FLAG_C;
            break;
        }
        default: {
            // handle unknown flags
            cerr << "Unknown flag: " << c << endl;
            break;
        }
    }
}

string ls_command::get_cur_dir() {
    // get current directory when m_cur_dir == .
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        return string(buffer);
    } else {
        m_logger->log(Logger::LogLevel::Error, "Failed to get current directory");
        return "";
    }

    return m_cur_dir;
}

const string& ls_command::get_help() {
    static const string help = {
        "Usage: ls -FLAGS <path>\n"
        "\t-a : do not ignore . && .. files\n"
        "\t-A : do not list the dot files\n"
        "\t-l : files long listing\n"
        "\t-C : colorize\n"
    };

    return help;
}

void ls_command::emit_cur_dir_changed() {
    m_logger->log(Logger::LogLevel::Info, "Current directory changed to %s", m_cur_dir.c_str());
}
