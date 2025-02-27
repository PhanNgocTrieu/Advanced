#include "ls_command.h"

ls_command* ls_command::instance = nullptr;

ls_command* ls_command::get_instance(int argc, char* argv[]) {
    if (instance == nullptr) {
        instance = new ls_command(argc, argv);
    }
    return instance;
}

ls_command::ls_command(int argc, char* argv[]) {
    if (argc > 1) {
        deserialize_argc(argc, argv);
    }
}

ls_command::ReetCode ls_command::deserialize_argc(int argc, char* argv[]) {
    // traverse the arguments
    for (int i = 1; i < argc; ++i) {
        // detecting the flag
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'h') {
                // print the help
                cout << get_help() << endl;
                return ReetCode::Success;
            }

            // using multiple flags
            if (strlen(argv[i]) > 2) {
                for (int j = 1; j < strlen(argv[i]); ++j) {
                    switch (argv[i][j]) {
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
                        default:
                            break;
                    }
                }
            }
            else {

            }
        }
        else {
            // detecting the directory
            m_cur_dir = string(argv[i]);
        }
    }
}

const string& ls_command::get_help() {
    return string (
        "\
        Usage: ls -d <directory> -f <files> \
        \n\t-d <directory> : directory to parse \
        \n\t-f <files> : pattern to catagorize the tail's file \
        "
    );
}
