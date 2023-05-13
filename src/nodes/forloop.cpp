#include "forloop.h"

#include "parser.h"

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
    llvm::BasicBlock *PreheaderBB = builder->GetInsertBlock();
    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(context.getGlobalContext(), "loopBB", function);

    builder->CreateBr(LoopBB);
    builder->SetInsertPoint(LoopBB);
    context.newScope(LoopBB);
    llvm::PHINode *IndVar = builder->CreatePHI(llvm::Type::getInt32Ty(context.getGlobalContext()), 2, "phi");
    IndVar->addIncoming(fromValue, PreheaderBB);

    //llvm::AllocaInst* alloca = new llvm::AllocaInst(llvm::Type::getInt32Ty(context.getGlobalContext()), 0, "i", context.currentBlock());
    //new llvm::StoreInst(IndVar, alloca, context.currentBlock());

    llvm::AllocaInst* alloca = VariableDeclaration::declareVariable(context, builder->getInt32Ty(), ident->getName(), loc);
    if (!alloca)
    {
        Node::printError(loc, "Ez a valtozonev mar letezik: " + ident->getName());
        context.addError();
        return nullptr;
    }

    builder->CreateStore(IndVar, alloca);

//    llvm::AllocaInst* alloca = builder->CreateAlloca(llvm::Type::getInt32Ty(context.getGlobalContext()), 0, "i");
//    builder->CreateStore(IndVar, alloca);
//    context.locals()[ident->getName()] = alloca;
//    context.setVarType(context.typeNameOf(alloca->getAllocatedType()), ident->getName());


    auto loopValue = loopBlock->codeGen(context);
    if (!loopValue)
    {
        Node::printError("A ciklus blokk nem kiertekelheto!");
        context.addError();
        return nullptr;
    }

    context.endScope(); // LoopBB

    llvm::Value *StepVal = builder->getInt32(1);
    llvm::Value *NextVal = builder->CreateAdd(IndVar, StepVal, "nextval");
    llvm::Value *EndCond = builder->CreateICmpULT(IndVar, toValue, "endcond");
    EndCond = builder->CreateICmpNE(EndCond, builder->getInt1(0), "loopcond");
    llvm::BasicBlock *LoopEndBB = builder->GetInsertBlock();
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(context.getGlobalContext(), "afterBB", function);
    builder->CreateCondBr(EndCond, LoopBB, AfterBB);
    builder->SetInsertPoint(AfterBB);
    IndVar->addIncoming(NextVal, LoopEndBB);
    context.setInsertPoint(AfterBB);
//    function->viewCFG();
    return AfterBB;
}

}
