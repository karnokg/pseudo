#include <cctype>
#include <fstream>
#include <cassert>
#include <filesystem>

#include "interpreter.h"

namespace Pseudo
{

Interpreter::Interpreter(const std::string& filename) : filename(filename) {}

Interpreter::~Interpreter() 
{
    delete program_block;
}

bool Interpreter::parse()
{
    if (filename.empty())
    {
        return false;
    }

    std::ifstream input(filename);
    if (!input.good())
    {
        std::cout << std::filesystem::path() << std::endl;
        std::cout << std::filesystem::current_path() << std::endl;
        std::cout << std::filesystem::temp_directory_path() << std::endl;
        std::cout << "Can't open file: " << filename << std::endl;
        return false;
    }

    return parse_helper(input);
}

void Interpreter::generateCode(const bool verbose, const bool run, const bool silent)
{
    NullBuffer noOut;
    CodeGenContext context(filename, silent ? noOut : std::cout);

    if (context.preProcessing(*program_block)) 
    {
        if (verbose)
        {
            context.printCodeGeneration(*program_block, std::cout);
        }

        if (context.generateCode(*program_block) && run) 
        {
            context.runCode();
        }
   }
}

bool Interpreter::parse_helper(std::istream& stream)
{
    Pseudo::Scanner scanner(&stream);
    Pseudo::Parser parser(scanner, *this);
    return parser.parse() == 0;
}

Pseudo::Block* Interpreter::get_ast_root()
{
    return program_block;
}

void Interpreter::set_ast_root(Pseudo::Block* root)
{
    program_block = root;
}

}
