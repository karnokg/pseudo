#include "compareoperator.h"
#include "node.h"
#include "parser.h"

namespace Pseudo
{
    using token = Parser::token;

    CompareOperator::CompareOperator(Expression* lhs, int op, Expression* rhs, location loc) 
                                   : op(op), lhs(lhs), rhs(rhs) { this->loc = loc; }

    llvm::Value* CompareOperator::codeGen(CodeGenContext& context) 
    {
        llvm::Value* rhsVal = rhs->codeGen(context);
        if (!rhsVal)
        {
            return nullptr;
        }

        llvm::Value* lhsVal = lhs->codeGen(context);
        if (!lhsVal)
        {
            return nullptr;
        }

        if ((lhsVal->getType() != llvm::Type::getDoubleTy(context.getGlobalContext())) && (lhsVal->getType() != context.getGenericIntegerType())) 
        {
            Node::printError(loc, "Binaris kifejezes bal oldala nem szam!");
            context.addError();
            return nullptr;
        }

        if ((rhsVal->getType() != llvm::Type::getDoubleTy(context.getGlobalContext())) && (rhsVal->getType() != context.getGenericIntegerType())) 
        {
            Node::printError(loc, "Binaris kifejezes jobb oldala nem szam!");
            context.addError();
            return nullptr;
        }

        if (rhsVal->getType() != lhsVal->getType())
        {
            auto castInstr = llvm::CastInst::getCastOpcode(rhsVal, true, llvm::Type::getDoubleTy(context.getGlobalContext()), true);
            rhsVal = llvm::CastInst::Create(castInstr, rhsVal, llvm::Type::getDoubleTy(context.getGlobalContext()), "castdb", context.currentBlock());
            castInstr = llvm::CastInst::getCastOpcode(lhsVal, true, llvm::Type::getDoubleTy(context.getGlobalContext()), true);
            lhsVal = llvm::CastInst::Create(castInstr, lhsVal, llvm::Type::getDoubleTy(context.getGlobalContext()), "castdb", context.currentBlock());
        }
     
        bool isDouble = rhsVal->getType() == llvm::Type::getDoubleTy(context.getGlobalContext());
        llvm::Instruction::OtherOps oinstr   = isDouble ? llvm::Instruction::FCmp : llvm::Instruction::ICmp;
     
        llvm::CmpInst::Predicate predicate;
        switch (op)
        {
        case token::T_GREATEREQ:
            predicate = isDouble ? llvm::CmpInst::FCMP_OGE : llvm::CmpInst::ICMP_SGE;
            break;
        case token::T_GREATER:
            predicate = isDouble ? llvm::CmpInst::FCMP_OGT : llvm::CmpInst::ICMP_SGT;
            break;
        case token::T_LESS:
            predicate = isDouble ? llvm::CmpInst::FCMP_OLT : llvm::CmpInst::ICMP_SLT;
            break;
        case token::T_LESSEQ:
            predicate = isDouble ? llvm::CmpInst::FCMP_OLE : llvm::CmpInst::ICMP_SLE;
            break;
        case token::T_EQ:
            predicate = isDouble ? llvm::CmpInst::FCMP_OEQ : llvm::CmpInst::ICMP_EQ;
            break;
        case token::T_NOTEQ:
            predicate = isDouble ? llvm::CmpInst::FCMP_ONE : llvm::CmpInst::ICMP_NE;
            break;
        default:
            Node::printError(loc, "Ismeretlen binaris operator!");
            context.addError();
            return nullptr;
        }
    
        context.getDbgInfo()->emitLocation(this);
        return llvm::CmpInst::Create(oinstr, predicate, lhsVal, rhsVal, "cmptmp", context.currentBlock());
    }

    NodeType CompareOperator::getType() 
    {
        return NodeType::Expression; 
    }

    std::string CompareOperator::str()
    {
        std::string ret = "[";

        switch (op)
        {
        case token::T_GREATEREQ:
            ret += ">=";
            break;
        case token::T_GREATER:
            ret += ">";
            break;
        case token::T_LESS:
            ret += "<";
            break;
        case token::T_LESSEQ:
            ret += "<=";
            break;
        case token::T_EQ:
            ret += "=";
            break;
        case token::T_NOTEQ:
            ret += "=/=";
            break;
        default:
            ret += "unknown";
            break;
        }

        return ret + "]";
    }

    void CompareOperator::accept(Visitor& visitor) 
    { 
        visitor.visitCompareOperator(this);
    }

    int CompareOperator::getOperator() const
    {
        return op;
    }

    Expression* CompareOperator::getLHS()
    {
        return lhs;
    }

    Expression* CompareOperator::getRHS()
    {
        return rhs;
    }
}
