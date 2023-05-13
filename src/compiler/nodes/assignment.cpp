#include "assignment.h"

#include "parser.h"
#include "location.hh"

namespace Pseudo
{
    Assignment::Assignment(Identifier* type, Identifier* id, Expression* expr, location loc)
        : type(type), id(id), expr(expr)
    {
        this->loc = loc;
    }

    Assignment::Assignment(Identifier* id, Expression* expr, location loc)
        : id(id), expr(expr)
    {
        this->loc = loc;
    }

    llvm::Value* Assignment::codeGen(CodeGenContext& context)
    {
        context.getDbgInfo()->emitLocation(this);
        context.getBuilder()->SetInsertPoint(context.currentBlock());

        llvm::Value* value = expr->codeGen(context);
        if (!value)
        {
            Node::printError(loc, "Ertekadas kifejezes nem legeneralhato.");
            context.addError();
            return nullptr;
        }

        llvm::AllocaInst* var = context.findVariable(id->getName());
        if (!var)
        {
            var = VariableDeclaration::declareVariable(context, value->getType(), id->getName(), loc);
        }

        llvm::Type* varType = var->getType()->getPointerElementType();
        llvm::Type* valType = value->getType();

        if (value->getType()->getTypeID() == varType->getTypeID()) 
        {
            // same type but different bit size
            if (value->getType()->getScalarSizeInBits() > varType->getScalarSizeInBits())
            {
                value = llvm::CastInst::CreateTruncOrBitCast(value, varType, "cast", context.currentBlock());
            } else if (value->getType()->getScalarSizeInBits() < varType->getScalarSizeInBits())
            {
                value = llvm::CastInst::CreateZExtOrBitCast(value, varType, "cast", context.currentBlock());
            }
        } else if (value->getType() != varType)
        {
            std::stringstream msg;
            msg << " Nem egyezo a tipusok! " << varType->getTypeID() << "(" << varType->getScalarSizeInBits() << ") = " << value->getType()->getTypeID()
                << "(" << value->getType()->getScalarSizeInBits() << ").";
            Node::printError(loc, msg.str());
            context.addError();
            return nullptr;
        }

        auto res = new llvm::StoreInst(value, var, false, context.currentBlock());
        return res;
    }
}
