#ifndef HEADER_LOG
    #define HEADER_LOG
    
    #ifdef DEBUG
        #include <string>
        #include <iostream>
        #include "blink-types.h"
        namespace log {
            void write(const std::string s);
            void writeByte(const byte s);
            void writeLine(const std::string s, const byte b);
        }

        #define LOG(x); log::write(x);
        #define LOG_BYTE(x); log::writeByte(x);
        #define LOG_LINE(x, y); log::writeLine(x,y);
    #else 
        #define LOG(x)
        #define LOG_BYTE(x)
        #define LOG_LINE(x,y)
    #endif
    
#endif