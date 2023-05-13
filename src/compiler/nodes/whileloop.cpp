#include "whileloop.h"

namespace Pseudo
{

llvm::Value* WhileLoop::codeGen(CodeGenContext& context)
{
    context.getDbgInfo()->emitLocation(this);

    llvm::Function* function = context.currentBlock()->getParent();
    llvm::BasicBlock* firstCondBlock = llvm::BasicBlock::Create(context.getGlobalContext(), "firstcond", function);
    llvm::BasicBlock* condBB = llvm::BasicBlock::Create(context.getGlobalContext(), "condition");
    llvm::BasicBlock* loopBB = llvm::BasicBlock::Create(context.getGlobalContext(), "loop");
    llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context.getGlobalContext(), "merge");

    llvm::BranchInst::Create(firstCondBlock, context.currentBlock());
    context.newScope(firstCondBlock);
    llvm::Value* firstCondValue = this->condition->codeGen(context);

    if (firstCondValue == nullptr)
    {
        Node::printError(loc, "A ciklus logikai kifejezese nem kiertekelheto.");
        context.addError();
        return nullptr;
    }

    if (!firstCondValue->getType()->isIntegerTy(1))
    {
        Node::printError(loc, "A ciklus feltetele nem logikai kifejezes.");
        context.addError();
        return nullptr;
    }
    llvm::BranchInst::Create(loopBB, mergeBB, firstCondValue, context.currentBlock());

    function->getBasicBlockList().push_back(condBB);
    context.endScope();

    context.newScope(condBB);
    llvm::Value* condValue = this->condition->codeGen(context);
    if (!condValue)
    {
        Node::printError(loc, "A ciklus logikai kifejezese nem kiertekelheto.");
        context.addError();
        return nullptr;
    }

    llvm::BranchInst::Create(loopBB, mergeBB, condValue, context.currentBlock());

    function->getBasicBlockList().push_back(loopBB);
    context.endScope();
    context.newScope(loopBB);
    llvm::Value* loopValue = this->loopBlock->codeGen(context);
    if (!loopValue)
    {
        Node::printError(loc, "A ciklus blokkja nem kiertekelheto.");
        context.addError();
        return nullptr;
    }

    llvm::BranchInst::Create(condBB, context.currentBlock());

    function->getBasicBlockList().push_back(mergeBB);
    context.endScope();
    context.setInsertPoint(mergeBB);

    //function->viewCFG();

    return mergeBB;
}
}