#ifndef FUNCTIONDECLARATION_H
#define FUNCTIONDECLARATION_H

#include "../node.h"

namespace Pseudo
{

class FunctionDeclaration : public Statement
{
public:
    Identifier* type;
    Identifier* id;
    VariableList* args;
    Block* block;

    FunctionDeclaration(Identifier* type, Identifier* id,
                        VariableList* args, Block* block, location loc)
        : type(type), id(id), args(args), block(block) { this->loc = loc; }
    void accept(Visitor& visitor) override;
    llvm::Value* codeGen(CodeGenContext& context) override;
    NodeType getType() override;
    std::string str() override;
    Block* getBody();
    VariableList* getParameters();
};

}

#endif // FUNCTIONDECLARATION_H
