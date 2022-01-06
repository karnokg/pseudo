#pragma once
#include <vector>
#include "../NExpression.h"
#include "NIdentifier.h"

class NMethodCall : public NExpression {
public:
    const NIdentifier& _id;
    std::vector<NExpression*> _arguments;
    NMethodCall(const NIdentifier& id, std::vector<NExpression*>& arguments) : _id(id), _arguments(arguments) { }
    NMethodCall(const NIdentifier& id) : _id(id) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};