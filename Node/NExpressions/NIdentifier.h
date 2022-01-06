#pragma once
#include "../NExpression.h"

class NIdentifier : public NExpression {
public:
    std::string _name;
    NIdentifier(const std::string& name) : _name(name) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};