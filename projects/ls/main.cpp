#include "ls_command.h"
#include "logger.h"

int main(int argc, char *argv[]) {

    try {
        // LOG_INIT("ls");
        // LOG_INFO("Hello, world!");
        ls_command* ls = ls_command::get_instance();
        if (ls == nullptr) {
            return ls_command::ReetCode::SystemError;
        }

        if (argc > 1) {
            ls->init_with_argc(argc, argv);
        }

        ls->execute();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
