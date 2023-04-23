#ifndef H_INTERPRETER
#define H_INTERPRETER

#include <string>
#include <cstddef>
#include <istream>
#include <iostream>

#include "scanner.h"
#include "parser.h"

class Block;

namespace Pseudo {

class Interpreter 
{
public:
    Interpreter();
    virtual ~Interpreter();

    int parse(const std::string& file);
    int parse(std::istream& iss);

    void set_ast_root(Block* root);
    Block* get_ast_root();

private:
    int parse_helper(std::istream& stream);
    Block* program_block = nullptr;
};

}

#endif