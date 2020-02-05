#ifdef DEBUG

    #include "log.h"
    
    namespace log {
        void write(const std::string s) {
            std::cout << "[LOG] " << s << std::endl;
        }
        void writeByte(const byte s) {
            std::cout << "[LOG] " << std::to_string(s) << std::endl;
        }
        void writeLine(const std::string s, const byte b) {
            std::cout << "[LOG] " << s << ": " << std::to_string(b) << std::endl;
        }
    }

#endif