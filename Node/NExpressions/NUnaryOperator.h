#pragma once
#include "../NExpression.h"

class NUnaryOperator : public NExpression {
public:
    int _op;
    NExpression& _expression;
    NUnaryOperator(const int op, NExpression& expression) : _op(op), _expression(expression) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};