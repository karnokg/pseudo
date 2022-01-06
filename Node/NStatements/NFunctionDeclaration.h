#pragma once
#include "../NStatement.h"
#include "../NExpressions/NIdentifier.h"
#include "../NExpressions/NBlock.h"
#include "NVariableDeclaration.h"

class NFunctionDeclaration : public NStatement {
public:
    const NIdentifier& _type;
    const NIdentifier& _id;
    std::vector<NVariableDeclaration*> _arguments;
    NBlock& _block;
    NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id, const std::vector<NVariableDeclaration*>& arguments, NBlock& block) : _type(type), _id(id), _arguments(arguments), _block(block) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};