#include "forloop.h"

#include "parser.h"
#include "binaryoperator.h"
#include "compareoperator.h"
#include "whileloop.h"

namespace Pseudo
{

llvm::Value* ForLoop::codeGen(CodeGenContext& context)
{
    context.getDbgInfo()->emitLocation(this);
    auto builder = context.getBuilder();
    builder->SetInsertPoint(context.currentBlock());

    auto fromValue = fromExpr->codeGen(context);
    auto toValue = toExpr->codeGen(context);

    if (!fromValue)
    {
        Node::printError("A \"tol\" nem kifejezes nem legeneralhato!");
        context.addError();
        return nullptr;
    }

    if (!toValue)
    {
        Node::printError("Az \"ig\" nem kifejezes nem legeneralhato!");
        context.addError();
        return nullptr;
    }

    if (fromValue->getType() != context.getGenericIntegerType())
    {
        Node::printError("A \"tol\" nem kifejezes egesz kifejezes!");
        context.addError();
        return nullptr;
    }

    if (toValue->getType() != context.getGenericIntegerType())
    {
        Node::printError("Az \"ig\" nem kifejezes egesz kifejezes!");
        context.addError();
        return nullptr;
    }

    llvm::Function* function = context.currentBlock()->getParent();
    llvm::BasicBlock* LoopBB = llvm::BasicBlock::Create(context.getGlobalContext(), "LoopBB", function);
    llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(context.getGlobalContext(), "MergeBB");

    llvm::BranchInst::Create(LoopBB, context.currentBlock());

    context.newScope(LoopBB);

    // TODO make it work if fromValue is greater than toValue


    auto toIdent = new Identifier("#" + ident->getName() + "_to", loc);

    VariableDeclaration(new Identifier(context.typeNameOf(fromValue->getType()), loc), ident, fromExpr, loc).codeGen(context);
    VariableDeclaration(new Identifier(context.typeNameOf(fromValue->getType()), loc), toIdent, toExpr, loc).codeGen(context);

    auto FromIsGreaterThanTo = new CompareOperator(ident, Parser::token::T_GREATER, toIdent, loc);

//    Conditional(
//        FromIsGreaterThanTo, 
//        new Block(StatementList
//        {
//            new Assignment(ident, toExpr, loc),
//            new Assignment(toIdent, fromExpr, loc),
//        }, loc),
//        new Block(StatementList
//        {
//            new Assignment(ident, fromExpr, loc),
//            new Assignment(toIdent, toExpr, loc),
//        }, loc), loc).codeGen(context);
//
    {
        auto IPlusOne = new BinaryOperator(ident, Parser::token::T_PLUS, new Integer(1, loc), loc);
        auto IMinusOne = new BinaryOperator(ident, Parser::token::T_MINUS, new Integer(1, loc), loc);

        auto StepStatement = new ConditionalExpression(
                FromIsGreaterThanTo, 
                new BinaryOperator(ident, Parser::token::T_MINUS, new Integer(1, loc), loc),
                new BinaryOperator(ident, Parser::token::T_PLUS, new Integer(1, loc), loc),
                loc);

        loopBlock->statements.push_back(new Assignment(ident, StepStatement, loc));

        auto Condition = new ConditionalExpression(
            FromIsGreaterThanTo, 
            new CompareOperator(ident, Parser::token::T_GREATER, toIdent, loc),
            new CompareOperator(ident, Parser::token::T_LESS, toIdent, loc),
            loc);

        //Expression* condition = new CompareOperator(new Identifier(ident->getName(), loc), Parser::token::T_LESSEQ, toIdent, loc);

        WhileLoop loop(Condition, loopBlock, loc);
        loop.codeGen(context);
        loopBlock->statements.pop_back();
        loopBlock->codeGen(context);

        // TODO REFACTOR THIS ASAP
    }

    llvm::BranchInst::Create(MergeBB, context.currentBlock());
    function->getBasicBlockList().push_back(MergeBB);
    context.endScope(); // LoopBB
    context.setInsertPoint(MergeBB);

    //function->viewCFG();

    return MergeBB;

    //llvm::Function* function = context.currentBlock()->getParent();
    //llvm::BasicBlock* LoopBB = llvm::BasicBlock::Create(context.getGlobalContext(), "LoopBB", function);
    //llvm::BasicBlock* StatementsBB = llvm::BasicBlock::Create(context.getGlobalContext(), "StatementsBB", function);
    //llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(context.getGlobalContext(), "MergeBB", function);

    //builder->CreateBr(LoopBB);
    //builder->SetInsertPoint(LoopBB);

    //llvm::AllocaInst* alloca = VariableDeclaration::declareVariable(context, builder->getInt32Ty(), ident->getName(), loc);
    //if (!alloca)
    //{
    //    alloca = builder->CreateLoad(fromValue->getType(), context.findVariable(ident->getName()));
    //}
    //else
    //{
    //    builder->CreateStore(fromValue, alloca);
    //}


    //context.newScope(LoopBB);
    //builder->SetInsertPoint(LoopBB);

    //auto CurrIndexValue = builder->CreateLoad(fromValue->getType(), alloca, "i");

    //auto statementsValue = loopBlock->codeGen(context);

    //if (!statementsValue)
    //{
    //    Node::printError(loc, "A ciklus blokk nem kiertekelheto!");
    //    context.addError();
    //    return nullptr;
    //}

    //auto NextIndex = builder->CreateAdd(CurrIndexValue, builder->getInt32(1));
    //builder->CreateStore(NextIndex, CurrIndexValue);
    //auto EndCondition = builder->CreateICmpNE(CurrIndexValue, toValue);
    //builder->CreateCondBr(EndCondition, LoopBB, MergeBB);
    //context.endScope(); // LoopBB

    //context.setInsertPoint(MergeBB);
    //function->viewCFG();

    //return MergeBB;























//
//    llvm::Function* function = context.currentBlock()->getParent();
//    llvm::BasicBlock *PreheaderBB = builder->GetInsertBlock();
//    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(context.getGlobalContext(), "loopBB", function);
//
//    builder->CreateBr(LoopBB);
//    builder->SetInsertPoint(LoopBB);
//    context.newScope(LoopBB);
//    llvm::PHINode *IndVar = builder->CreatePHI(llvm::Type::getInt32Ty(context.getGlobalContext()), 2, "phi");
//    IndVar->addIncoming(fromValue, PreheaderBB);
//
//    //llvm::AllocaInst* alloca = new llvm::AllocaInst(llvm::Type::getInt32Ty(context.getGlobalContext()), 0, "i", context.currentBlock());
//    //new llvm::StoreInst(IndVar, alloca, context.currentBlock());
//
//    llvm::AllocaInst* alloca = VariableDeclaration::declareVariable(context, builder->getInt32Ty(), ident->getName(), loc);
//    if (!alloca)
//    {
//        Node::printError(loc, "Ez a valtozonev mar letezik: " + ident->getName());
//        context.addError();
//        return nullptr;
//    }
//
//    builder->CreateStore(IndVar, alloca);
//
////    llvm::AllocaInst* alloca = builder->CreateAlloca(llvm::Type::getInt32Ty(context.getGlobalContext()), 0, "i");
////    builder->CreateStore(IndVar, alloca);
////    context.locals()[ident->getName()] = alloca;
////    context.setVarType(context.typeNameOf(alloca->getAllocatedType()), ident->getName());
//
//    llvm::BasicBlock* StatementsBB = llvm::BasicBlock::Create(context.getGlobalContext(), "statementBB", function);
//    builder->CreateBr(StatementsBB);
//    context.newScope(StatementsBB);
//    auto loopValue = loopBlock->codeGen(context);
//    if (!loopValue)
//    {
//        context.endScope();
//        Node::printError("A ciklus blokk nem kiertekelheto!");
//        context.addError();
//        return nullptr;
//    }
//    
//    context.endScope();
//
//    context.endScope(); // LoopBB
//
//    llvm::Value *StepVal = builder->getInt32(1);
//    llvm::Value *NextVal = builder->CreateAdd(IndVar, StepVal, "nextval");
//    llvm::Value *EndCond = builder->CreateICmpULT(IndVar, toValue, "endcond");
//    EndCond = builder->CreateICmpNE(EndCond, builder->getInt1(0), "loopcond");
//    llvm::BasicBlock *LoopEndBB = builder->GetInsertBlock();
//
//    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(context.getGlobalContext(), "afterBB", function);
//    builder->CreateCondBr(EndCond, LoopBB, AfterBB);
//    builder->SetInsertPoint(AfterBB);
//    IndVar->addIncoming(NextVal, LoopEndBB);
//    context.setInsertPoint(AfterBB);
//    function->viewCFG();
//    return AfterBB;
}

}
