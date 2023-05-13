#ifndef H_FORLOOP
#define H_FORLOOP

#include "node.h"

namespace Pseudo
{

class CompareOperator;

class ForLoop : public Statement
{
public:
    Identifier* ident;
    Expression* fromExpr;
    Expression* toExpr;
    Block* loopBlock;

    ForLoop(Identifier* ident, Expression* fromExpr, Expression* toExpr, Block* loopBlock, location loc)
        : ident(ident), fromExpr(fromExpr), toExpr(toExpr), loopBlock(loopBlock) 
    {
        this->loc = loc;
    }

    llvm::Value* codeGen(CodeGenContext& context) override;

    NodeType getType() override
    {
        return NodeType::Expression;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitForLoop(this);
    }

    Expression* getFromExpr()
    {
        return fromExpr;
    }

    Expression* getToExpr()
    {
        return toExpr;
    }

    Block* getLoopBlock()
    {
        return loopBlock;
    }
};

}

#endif
