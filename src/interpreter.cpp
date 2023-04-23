#include <cctype>
#include <fstream>
#include <cassert>
#include <filesystem>

#include "interpreter.h"

Pseudo::Interpreter::Interpreter() {}

Pseudo::Interpreter::~Interpreter() 
{
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
        std::cout << std::filesystem::path() << std::endl;
        std::cout << std::filesystem::current_path() << std::endl;
        std::cout << std::filesystem::temp_directory_path() << std::endl;
        std::cout << "can't open file: " << filename << std::endl;
        return 1;
    }

    return parse_helper(input);
}

int Pseudo::Interpreter::parse_helper(std::istream& stream)
{
    Pseudo::Scanner scanner(&stream);
    Pseudo::Parser parser(scanner, *this);
    return parser.parse();
}

Pseudo::Block* Pseudo::Interpreter::get_ast_root()
{
    return program_block;
}

void Pseudo::Interpreter::set_ast_root(Pseudo::Block* root)
{
    program_block = root;
}
