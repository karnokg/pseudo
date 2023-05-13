#include "unaryoperator.h"

#include "parser.h"

namespace Pseudo
{

using token = Parser::token;

UnaryOperator::UnaryOperator(int op, Expression* rhs, location loc)
    : op(op), rhs(rhs) { this->loc = loc; }

llvm::Value* UnaryOperator::codeGen(CodeGenContext &context)
{
    llvm::Instruction::BinaryOps instruction;

    switch (op)
    {
        case token::T_NOT:
            instruction = llvm::Instruction::Xor;
        default:
            std::cout << "Ismeretlen unaris operator";
            return nullptr;
    }

    llvm::Value* rhsValue = rhs->codeGen(context);

    if (!rhsValue)
    {
        Node::printError(loc, "Az unaris kifejezes jobb oldala nem kiertekelheto");
        context.addError();
        return nullptr;
    }

    if (!rhsValue->getType()->isIntegerTy())
    {
        Node::printError(loc, "Az unaris kifejezes jobb oldala nem egesz!");
        context.addError();
        return nullptr;
    }

    llvm::Value* lhsValue = llvm::ConstantInt::get(llvm::IntegerType::get(context.getGlobalContext(), context.getGenericIntegerType()->getIntegerBitWidth()), llvm::StringRef("-1"), 10);

    context.getDbgInfo()->emitLocation(this);

    return llvm::BinaryOperator::Create(instruction, lhsValue, rhsValue, "unarytmp", context.currentBlock());
}

NodeType Pseudo::UnaryOperator::getType() {
    return NodeType::Expression;
}

std::string UnaryOperator::str()
{
    std::string out;

    switch (op)
    {
    case token::T_NOT:
        out += "NOT";
        break;
    default:
        out += "ismeretlen";
        break;
    }

    return out;
}

Expression* UnaryOperator::getRHS()
{
    return rhs;
}

}

