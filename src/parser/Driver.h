#ifndef SNOW_DRIVER_H
#define SNOW_DRIVER_H

#include <string>
#include <iostream>
#include "Scanner.h"
#include "Codegen.h"

namespace snow {

    class Driver
    {
    public:
        inline Driver() {};

        std::string streamname;
        Scanner* lexer;

        bool parse_stream(std::istream& in, const std::string& sname = "stream input");
        bool parse_string(const std::string& input, const std::string& sname = "string stream");
        bool parse_file(const std::string& filename);
        inline void error(const location& l, const std::string& m) { std::cerr << l << ": " << m << std::endl; }
        inline void error(const std::string& m) { std::cerr << m << std::endl; }
    };

}
#endif
