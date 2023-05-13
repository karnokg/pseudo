#ifndef BINARYOPERATOR_H
#define BINARYOPERATOR_H

#include "node.h"

namespace Pseudo
{

class BinaryOperator : public Expression
{
public:
    int op;
    Expression* lhs;
    Expression* rhs;

    BinaryOperator(Expression* lhs, int op, Expression* rhs, location loc);
    llvm::Value* codeGen(CodeGenContext& context) override;
    NodeType getType() override;
    std::string str() override;

    Expression* getLHS();
    Expression* getRHS();
};

}
#endif // BINARYOPERATOR_H
