#ifndef H_COMPAREOPERATOR
#define H_COMPAREOPERATOR

#include "node.h"

namespace Pseudo
{

class CompareOperator : public Expression
{
private:
    int op{0};
    Expression* lhs{nullptr};
    Expression* rhs{nullptr};
public:
    CompareOperator(Expression* lhs, int op, Expression* rhs, location loc);

    llvm::Value* codeGen(CodeGenContext& context) override;
    NodeType getType() override;
    std::string str() override;
    void accept(Visitor& v) override;

    int getOperator() const;
    Expression* getLHS();
    Expression* getRHS();
};

}

#endif 
