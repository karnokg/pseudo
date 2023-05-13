#ifndef H_ASSIGNMENT
#define H_ASSIGNMENT

#include "../node.h"
#include "../visitor.h"

#include "location.hh"

namespace Pseudo
{

class Assignment : public Statement
{
public:
    Identifier* type;

    Identifier* id;
    Expression* expr;

    Assignment(Identifier* type, Identifier* id, Expression* expr, location loc);
    Assignment(Identifier* id, Expression* expr, location loc);

    llvm::Value* codeGen(CodeGenContext& context);

    void accept(Visitor& visitor)
    {
        visitor.visitAssignment(this);
    }

    NodeType getType() override
    {
        return NodeType::Expression;
    }

    std::string str()
    {
        return "[ Assignment Statement (id=" + id->getName() +") ]";
    }

    Expression* getExpression()
    {
        return expr;
    }
};

}

#endif