#ifndef H_INTERPRETER
#define H_INTERPRETER

#include <string>
#include <cstddef>
#include <istream>

#include "scanner.h"
#include "parser.h"

class Block;

namespace Pseudo {

class Interpreter 
{
public:
    Interpreter() = default;
    virtual ~Interpreter();

    int parse(const std::string& file);
    int parse(std::istream& iss);

    void set_ast_root(Block* root);
    Block* get_ast_root();

private:
    int parse_helper(std::istream& stream);
    Pseudo::Parser* parser = nullptr;
    Pseudo::Scanner* scanner = nullptr;

    Block* program_block = nullptr;
};

}

#endif