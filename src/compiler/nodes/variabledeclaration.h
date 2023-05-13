#ifndef VARIABLEDECLARATION_H
#define VARIABLEDECLARATION_H

#include "node.h"

#include "location.hh"

namespace Pseudo
{

class VariableDeclaration : public Statement
{
public:
    Identifier* type{nullptr};
    Identifier* id{nullptr};
    Expression* assignmentExpr{nullptr};

    VariableDeclaration(Identifier* id, location loc);
    VariableDeclaration(Identifier* type, Identifier* id, location loc);
    VariableDeclaration(Identifier* type, Identifier* id,
                        Expression* assignmentExpr, location loc);

    llvm::Value* codeGen(CodeGenContext& context) override;
    NodeType getType() override;
    void accept(Visitor& visitor) override;
    std::string str() override;

    const Identifier& getTypeIdentifier();

    std::string getVariableTypeName();
    std::string getVariableName();

    Expression* getAssignment();
    bool hasAssignmentExpr() const;

    /// This function declares a variable. Only returns nullptr if variable already exists.
    static llvm::AllocaInst* declareVariable(CodeGenContext& context, llvm::Type* ty, const std::string& varName, location loc);
};

}

#endif // VARIABLEDECLARATION_H
