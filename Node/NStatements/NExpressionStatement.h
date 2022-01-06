#pragma once
#include "../NStatement.h"
#include "../NExpression.h"

class NExpressionStatement : public NStatement {
public:
    NExpression& _expression;
    NExpressionStatement(NExpression& expression) : _expression(expression) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};