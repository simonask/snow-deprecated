#include <fstream>
#include <sstream>

#include "Driver.h"
#include "Scanner.h"
#include "runtime/Runtime.h"

namespace snow {
    RefPtr<ast::FunctionDefinition> Driver::parse(std::istream& in, const std::string& name) {
        Driver driver;
        driver.streamname = name;
        Scanner scanner(&in);
        driver.lexer = &scanner;
        Parser parser(driver);
        parser.parse();
        
        return driver.scope;
    }
    
    RefPtr<ast::FunctionDefinition> Driver::parse(const std::string& input, const std::string& name) {
        std::istringstream iss(input);
        return parse(iss, name);
    }
}
