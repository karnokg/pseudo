#ifndef H_VECTOR
#define H_VECTOR

#include "node.h"

namespace Pseudo
{

class Vector : public Expression
{
public:
    Identifier* type{nullptr};
    ExpressionList* exprs{nullptr};
    Expression* maxSizeExpr{nullptr};

    //Vector(Identifier* id) : id(id) {}
    Vector(Identifier* type, ExpressionList* exprs, Expression* maxSizeExpr, location loc);
    Vector(Identifier* type, Expression* maxSizeExpr, location loc);

    llvm::Value* codeGen(CodeGenContext& context) override;
    llvm::Value* codeGenOLD(CodeGenContext& context);

    NodeType getType() override
    {
        return NodeType::Vector;
    }

    std::string str() override
    {
        return "[ Vector(type=" + type->getName() + " ]";
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitVector(this);
    }

    ExpressionList* getExpressions()
    {
        return exprs;
    }
};
}
#endif