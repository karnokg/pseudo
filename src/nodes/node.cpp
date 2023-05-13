#include "node.h"


namespace Pseudo
{

void Node::printError(const location& loc, const std::string& str)
{
    std::cout << "Hiba: " << str << " ITT: Sor " << loc.begin.line << ", Oszlop " << loc.begin.column << std::endl;
}

}
