#ifndef SNOW_DRIVER_H
#define SNOW_DRIVER_H

#include <string>
#include <iostream>
#include "Scanner.h"
#include "codegen/ASTNode.h"
#include "base/RefPtr.h"


namespace snow {

    class Driver
    {
    public:
        inline Driver() {};

        std::string streamname;
        RefPtr<ast::FunctionDefinition> scope;
        Scanner* lexer;

        inline void error(const location& l, const std::string& m) { std::cerr << l << ": " << m << std::endl; }
        inline void error(const std::string& m) { std::cerr << m << std::endl; }

    public:
        static RefPtr<ast::FunctionDefinition> parse(std::istream& in, const std::string& sname = "stream input");
        static RefPtr<ast::FunctionDefinition> parse(const std::string& input, const std::string& name = "stream input");
    };

}
#endif
