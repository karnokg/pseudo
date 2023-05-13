#ifndef UNARYOPERATOR_H
#define UNARYOPERATOR_H

#include "node.h"

namespace Pseudo
{

class UnaryOperator : public Expression
{
public:
    int op;
    Expression* rhs;

    UnaryOperator(int op, Expression* rhs, location loc);
    llvm::Value* codeGen(CodeGenContext& context) override;
    NodeType getType() override;
    std::string str() override;
    Expression* getRHS();
};

}

#endif // UNARYOPERATOR_H
