#pragma once
#include "../NStatement.h"

class NVariableDeclaration : public NStatement {
public:
    const NIdentifier& _type;
    NIdentifier& _id;
    NExpression* _assignmentExpr;

    NVariableDeclaration(const NIdentifier& type, NIdentifier& id) : _type(type), _id(id) { }
    NVariableDeclaration(const NIdentifier& type, NIdentifier& id, NExpression* assignmentExpr) : _type(type), _id(id), _assignmentExpr(assignmentExpr) { }

    virtual llvm::Value* codeGen(CodeGenContext& context);
};