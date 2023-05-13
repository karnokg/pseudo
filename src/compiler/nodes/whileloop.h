#ifndef H_WHILELOOP
#define H_WHILELOOP

#include "node.h"

namespace Pseudo
{

class WhileLoop : public Statement
{
public:
    Expression* condition;
    Block* loopBlock;

    WhileLoop(Expression* condition, Block* loopBlock, location loc)
        : condition(condition), loopBlock(loopBlock) { this->loc = loc; }

    llvm::Value* codeGen(CodeGenContext& context) override;

    NodeType getType() override
    {
        return NodeType::Expression;
    }

    std::string str() override
    {
        return "[ WhileLoop ]";
    }

    Expression* getCondition()
    {
        return condition;
    }

    Block* getLoopBlock()
    {
        return loopBlock;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitWhileLoop(this);
    }
};
}


#endif