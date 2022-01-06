#pragma once
#include "../NExpression.h"

class NRational : public NExpression {
public:
    double _value;
    NRational(const double value) : _value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};