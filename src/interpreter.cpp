#include <cctype>
#include <fstream>
#include <cassert>

#include "interpreter.h"

Pseudo::Interpreter::~Interpreter() 
{
    delete scanner;
    scanner = nullptr;
    delete parser;
    parser = nullptr;
    delete program_block;
    program_block = nullptr;
}

int Pseudo::Interpreter::parse(const std::string& filename)
{
    if (filename.empty())
    {
        return 1;
    }

    std::ifstream input(filename);
    if (!input.good())
    {
        std::cout << "can't open file: " << filename << std::endl;
        return 1;
    }

    return parse_helper(input);
}

int Pseudo::Interpreter::parse_helper(std::istream& stream)
{
    delete scanner;
    try
    {
        scanner = new Pseudo::Scanner(&stream);
    }
    catch(const std::bad_alloc& e)
    {
        std::cerr << "Can't allocate scanner: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    delete parser;
    try
    {
        parser = new Pseudo::Parser(*scanner, *this);
    }
    catch(const std::bad_alloc& e)
    {
        std::cerr << "Can't allocate parser: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return parser->parse();
}

Pseudo::Block* Pseudo::Interpreter::get_ast_root()
{
    return program_block;
}

void Pseudo::Interpreter::set_ast_root(Pseudo::Block* root)
{
    program_block = root;
}