#pragma once
#include "../NExpression.h"

class NInteger : public NExpression {
public:
    long long _value;
    NInteger(const long long value) : _value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};