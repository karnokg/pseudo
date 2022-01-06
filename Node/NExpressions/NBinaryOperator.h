#pragma once
#include "../NExpression.h"

class NBinaryOperator : public NExpression {
public:
    int _op;
    NExpression& _lhs, _rhs;
    NBinaryOperator(int op, NExpression& lhs, NExpression& rhs) : _op(op), _lhs(lhs), _rhs(rhs) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};