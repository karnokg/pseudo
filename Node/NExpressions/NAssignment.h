#pragma once
#include "../NExpression.h"
#include "NIdentifier.h"

class NAssignment : public NExpression {
public:
    NIdentifier& _lhs;
    NExpression& _rhs;

    NAssignment(NIdentifier& lhs, NExpression& rhs) : _lhs(lhs), _rhs(rhs) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};