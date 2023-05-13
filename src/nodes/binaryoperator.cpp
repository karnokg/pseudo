#include "binaryoperator.h"
#include "node.h"
#include "parser.h"

namespace Pseudo {

using token = Parser::token;

Pseudo::BinaryOperator::BinaryOperator(Expression *lhs, int op, Expression *rhs, location loc)
    : op(op), lhs(lhs), rhs(rhs) { this->loc = loc; }

llvm::Value* Pseudo::BinaryOperator::codeGen(CodeGenContext &context)
{
    llvm::Value* lhsValue = lhs->codeGen(context);
    llvm::Value* rhsValue = rhs->codeGen(context);

    if (!lhsValue || !rhsValue)
    {
        return nullptr;
    }

    context.getDbgInfo()->emitLocation(this);

    auto Ty = rhsValue->getType();

    if (rhsValue->getType() != lhsValue->getType())
    {
        std::cout << "A ket tipus nem egyezik, castolas";
        auto doubleTy = llvm::Type::getDoubleTy(context.getGlobalContext());
        auto cinstruction = llvm::CastInst::getCastOpcode(rhsValue, true, doubleTy, true);
        rhsValue = llvm::CastInst::Create(cinstruction, rhsValue, doubleTy, "castrational", context.currentBlock());
        cinstruction = llvm::CastInst::getCastOpcode(lhsValue, true, doubleTy, true);
        lhsValue = llvm::CastInst::Create(cinstruction, lhsValue, doubleTy, "castrational", context.currentBlock());
    }

    bool isDoubleTy = rhsValue->getType()->isFloatingPointTy();
    bool isIntegerTy = rhsValue->getType()->isIntegerTy();

    if ((isDoubleTy || isIntegerTy) && (op == token::T_AND || op == token::T_OR))
    {
        std::cout << "Az 'es' es 'or' operator csak bool tipussal hasznalhato!";
        return nullptr;
    }

    llvm::Instruction::BinaryOps instruction;

    switch (op) {

    case token::T_PLUS:
        isDoubleTy ? instruction = llvm::Instruction::FAdd : instruction = llvm::Instruction::Add;
        break;
    case token::T_MINUS:
        isDoubleTy ? instruction = llvm::Instruction::FSub : instruction = llvm::Instruction::Sub;
        break;
    case token::T_MUL:
        isDoubleTy ? instruction = llvm::Instruction::FMul : instruction = llvm::Instruction::Mul;
        break;
    case token::T_DIV:
        isDoubleTy ? instruction = llvm::Instruction::FDiv : instruction = llvm::Instruction::SDiv;
        break;
    case token::T_MODULUS:
        isDoubleTy ? instruction = llvm::Instruction::FRem : instruction = llvm::Instruction::SRem;
        break;
    case token::T_AND:
        instruction = llvm::Instruction::And;
        break;
    case token::T_OR:
        instruction = llvm::Instruction::Or;
        break;
    default:
        std::cout << "Nincs ilyen binaris operator!";
        return nullptr;
    }

    return llvm::BinaryOperator::Create(instruction, lhsValue, rhsValue, "mathtmp", context.currentBlock());
}

Pseudo::NodeType Pseudo::BinaryOperator::getType() { return NodeType::Expression; }

std::string Pseudo::BinaryOperator::str()
{
    std::string ret = "[ ";
    switch (op) {
    case token::T_PLUS:
        ret += "+";
        break;
    case token::T_MINUS:
        ret += "-";
        break;
    case token::T_MUL:
        ret += "*";
        break;
    case token::T_DIV:
        ret += "/";
        break;
    case token::T_MODULUS:
        ret += "Mod";
        break;
    case token::T_AND:
        ret += "And";
        break;
    case token::T_OR:
        ret += "Or";
        break;
    default:
        ret += "Unknown";
    }
    return ret + " ]";
}

Expression* BinaryOperator::getLHS()
{
    return lhs;
}

Expression* BinaryOperator::getRHS()
{
    return rhs;
}

}
