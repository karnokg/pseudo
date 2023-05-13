#include "functiondeclaration.h"
#include "variabledeclaration.h"

namespace Pseudo
{

void FunctionDeclaration::accept(Visitor& visitor)
{
    return visitor.visitFunctionDeclaration(this);
}

llvm::Value* FunctionDeclaration::codeGen(CodeGenContext& context)
{
    auto DBuilder = context.getDBuilder();

    std::vector<llvm::Type*> argTypes;

    for (auto varDecl : *args)
    {
        llvm::Type* ty = context.typeOf(varDecl->getTypeIdentifier());

        if (ty->isStructTy()) {
            ty = llvm::PointerType::get(ty, 0);
        }

        argTypes.push_back(ty);
    }

    llvm::FunctionType* ftype = llvm::FunctionType::get(context.typeOf(*type), argTypes, false);

    std::string functionName = id->getName();

    llvm::Function* function = llvm::Function::Create(ftype, llvm::GlobalValue::InternalLinkage, functionName.c_str(), context.getModule());

    llvm::BasicBlock* bblock = llvm::BasicBlock::Create(context.getGlobalContext(), "entry", function, 0);

    // DEBUG informations start

    llvm::DIFile* Unit = DBuilder->createFile(context.getDbgInfo()->TheCU->getFilename(),
                                         context.getDbgInfo()->TheCU->getDirectory());
    llvm::DIScope* FContext = Unit;
    unsigned LineNo = 0;
    unsigned ScopeLine = LineNo;

    auto debug = function->getReturnType();

    auto SubprogramType = context.getDbgInfo()->createFunctionType(function->getReturnType(), argTypes);

    llvm::DISubprogram *SP = DBuilder->createFunction(
         FContext, bblock->getName(), llvm::StringRef(), Unit, LineNo,
                SubprogramType, ScopeLine,
         llvm::DINode::FlagPrototyped, llvm::DISubprogram::SPFlagDefinition);

    function->setSubprogram(SP);

    context.newScope(bblock, SP, ScopeType::FunctionDeclaration);

    context.getDbgInfo()->emitLocation<FunctionDeclaration>(nullptr);

    //DEBUG informations end

    // Put the parameter onto the stack in order to be accessed in the function.
    llvm::Function::arg_iterator actualArgs = function->arg_begin();

    for (size_t argIdx = 0; argIdx < args->size(); ++argIdx)
    {
        auto varDecl = (*args)[argIdx];

        llvm::AllocaInst* alloca = llvm::dyn_cast<llvm::AllocaInst>(varDecl->codeGen(context));
        std::string valName = varDecl->getVariableName();

        if (alloca)
        {
            if (alloca->getAllocatedType()->isPointerTy())
            {
                valName += "_addr";
            }

            actualArgs->setName(valName);
            new llvm::StoreInst(&(*actualArgs), alloca, context.currentBlock());
        }

        ++actualArgs;
    }

    context.getDbgInfo()->emitLocation(this);

    // Generate the function body.
    auto blockValue = block->codeGen(context);

    if(blockValue == nullptr)
    {
        Node::printError(id->getName() + " fuggveny nem kiertekelheto!");
        context.addError();
        return nullptr;
    }

    auto retTy = blockValue->getType();

    // If the function doesn't have a return type and doesn't have a return statement, make a ret void.
    // Obsolete default is var.
    if( type->getName() == "void" ) {
        if( context.currentBlock()->getTerminator() == nullptr ) {
            llvm::ReturnInst::Create( context.getGlobalContext(), 0, context.currentBlock() );
        }
    }

    //// If the now return instruction is generated so far ...
    //if( context.currentBlock()->getTerminator() == nullptr )
    //{
    //    if(retTy->isVoidTy())
    //    {
    //        llvm::ReturnInst::Create( context.getGlobalContext(), 0, context.currentBlock() );
    //    }
    //}

    //context.currentBlock()->getParent()->viewCFG();

    DBuilder->finalizeSubprogram(SP);
    context.endScope();

    //function->viewCFG();


    if (llvm::verifyFunction(*function, &llvm::outs()))
    {
        Node::printError(loc, "Fuggveny generalasi hiba tortent!");
        context.addError();
        return nullptr;
    }

    return function;
}

NodeType FunctionDeclaration::getType()
{
    return NodeType::Function;
}

std::string FunctionDeclaration::str()
{
    return "[ function declaration(type=" + type->getName() + ", id=" + id->getName() + ") ]";
}

Block* FunctionDeclaration::getBody()
{
    return block;
}

VariableList* FunctionDeclaration::getParameters()
{
    return args;
}

}
