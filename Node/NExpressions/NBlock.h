#pragma once
#include <vector>
#include "../NExpression.h"
#include "../NStatement.h"

class NBlock : public NExpression {
public:
    std::vector<NStatement*> _statements;
    NBlock() { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};