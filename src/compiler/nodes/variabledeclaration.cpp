#include "variabledeclaration.h"

#include "assignment.h"

namespace Pseudo
{

VariableDeclaration::VariableDeclaration(Identifier* id, location loc) : id(id) 
{
    this->loc = loc;
}

VariableDeclaration::VariableDeclaration(Identifier* type, Identifier* id, location loc)
    : type(type), id(id) { this->loc = loc; }

VariableDeclaration::VariableDeclaration(Identifier* type, Identifier* id, Expression* assignmentExpr, location loc)
    : type(type), id(id), assignmentExpr(assignmentExpr) { this->loc = loc; }

llvm::Value* VariableDeclaration::codeGen(CodeGenContext& context)
{
    llvm::Value* value = nullptr;

    llvm::Type* ty = context.typeOf(*type);

    context.getDbgInfo()->emitLocation(this);
    
    value = declareVariable(context, ty, id->getName(), loc);

    if (!value)
    {
        Node::printError(loc, "Mar letezik ez a valtozonev: " + id->getName());
        context.addError();
        return nullptr;
    }

    if (assignmentExpr)
    {
        Assignment assn(id, assignmentExpr, loc);
        assn.codeGen(context);
    }

    return value;
}

llvm::AllocaInst* VariableDeclaration::declareVariable(CodeGenContext& context, llvm::Type* ty, const std::string& varName, location loc)
{
    context.getDbgInfo()->emitLocation(loc);

    auto Builder = context.getBuilder();
    Builder->SetInsertPoint(context.currentBlock());
    auto DBuilder = context.getDBuilder();

    if (context.findVariable(varName))
    {
        return nullptr;
    }

    if (ty->isVoidTy())
    {
        Node::printError(loc, "Void ertekkel nem lehet deklaralni valtozot!");
        context.addError();
        return nullptr;
    }

    llvm::AllocaInst* alloca = new llvm::AllocaInst(ty, 0, varName.c_str(), context.currentBlock());

    context.locals()[varName] = alloca;
    context.setVarType(context.typeNameOf(ty), varName);

    llvm::DIFile* Unit = context.getDbgInfo()->getUnit();
    // Create a debug descriptor for the variable.
    llvm::DILocalVariable* D = DBuilder->createParameterVariable(
         context.currentDebugScope(), varName, 0, Unit, loc.begin.line, context.getDbgInfo()->debugTypeOf(ty),
         true);

    auto SP = context.currentDebugScope();
    DBuilder->insertDeclare(alloca, D, DBuilder->createExpression(),
                             llvm::DILocation::get(SP->getContext(), loc.begin.line, 0, SP),
                             Builder->GetInsertBlock());


    return alloca;
}

NodeType VariableDeclaration::getType()
{
    return NodeType::Expression;
}

void VariableDeclaration::accept(Visitor& visitor)
{
    visitor.visitVariableDeclaration(this);
}

std::string VariableDeclaration::str()
{
    return "[ variable declaration (name=" + id->getName() + ", type=" + (type ? type->getName() : "?") + ") ]";
}

const Identifier& VariableDeclaration::getTypeIdentifier()
{
    return *type;
}

std::string VariableDeclaration::getVariableTypeName()
{
    return type->getName();
}

std::string VariableDeclaration::getVariableName()
{
    return id->getName();
}

Expression* VariableDeclaration::getAssignment()
{
    return assignmentExpr;
}

bool VariableDeclaration::hasAssignmentExpr() const
{
    return assignmentExpr;
}

}
