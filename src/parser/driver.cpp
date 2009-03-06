#include <fstream>
#include <sstream>

#include "Driver.h"
#include "Scanner.h"

namespace snow {

    bool Driver::parse_stream(std::istream& in, const std::string& sname) {
        streamname = sname;

        Scanner scanner(&in);
        this->lexer = &scanner;

        Parser parser(*this);
        return (parser.parse() == 0);
    }

    bool Driver::parse_file(const std::string &filename) {
        std::ifstream in(filename.c_str());
        if (!in.good()) return false;
        return parse_stream(in, filename);
    }

    bool Driver::parse_string(const std::string &input, const std::string& sname) {
        std::istringstream iss(input);
        return parse_stream(iss, sname);
    }

}
