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

class NullBuffer : public std::ostream
{
public:
  int overflow(int c) { return c; }
};

class Interpreter 
{
public:
    Interpreter(const std::string& filename);
    virtual ~Interpreter();

    bool parse();

    void generateCode(const bool verbose, const bool run, const bool silent);

    void set_ast_root(Block* root);
    Block* get_ast_root();

private:
    bool parse(std::istream& iss);
    std::string filename;

    bool parse_helper(std::istream& stream);
    Block* program_block = nullptr;
};

}

#endif