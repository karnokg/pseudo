#pragma once
#include "../NExpression.h"

class NBoolean : public NExpression {
public:
    bool _value;
    NBoolean(const bool value) : _value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};