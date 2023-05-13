#include <filesystem>
#include <typeinfo>
#include "CodeGenContext.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include <stdarg.h>
#include <stdio.h>
#include <algorithm>
#include <cmath>

#include "node.h"
#include "nodes/string.h"

#include "externfunctions.h"
#include "visitorsyntaxcheck.h"
#include "visitorprintast.h"

namespace Pseudo
{

CodeGenContext::CodeGenContext(const std::string& filename, std::ostream& outs) : outs(outs)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();

    module = new llvm::Module("pseudo", llvmContext);
    module->addModuleFlag(llvm::Module::Warning, "Dwarf Version", 5);
    module->addModuleFlag(llvm::Module::Error, "Debug Info Version",
                             llvm::DEBUG_METADATA_VERSION);

    setupBuiltIns();

    Builder = new llvm::IRBuilder<>(getGlobalContext());
    DBuilder = new llvm::DIBuilder(*module);
    DbgInfo = new DebugInfo(filename, *this);
}

#define MAKE_LLVM_EXTERNAL_NAME(a) #a
void CodeGenContext::setupBuiltIns()
{
    intType = getGenericIntegerType();
    rationalType = llvm::Type::getDoubleTy(getGlobalContext());
    stringType = llvm::Type::getInt8PtrTy(getGlobalContext());
    boolType = llvm::Type::getInt1Ty(getGlobalContext());
    voidType = llvm::Type::getVoidTy(getGlobalContext());
    stringType = llvm::Type::getInt8PtrTy(getGlobalContext());
 
    intVectorType = llvm::Type::getInt32PtrTy(getGlobalContext());
    rationalVectorType = llvm::Type::getDoublePtrTy(getGlobalContext());
    boolVectorType = llvm::Type::getInt1PtrTy(getGlobalContext());
 
    llvmTypeMap["integer"] = intType;
    llvmTypeMap["rational"] = rationalType;
    llvmTypeMap["string"] = stringType;
    llvmTypeMap["boolean"] = boolType;
    llvmTypeMap["void"] = voidType;
 
    llvmTypeMap["vec_integer"] = intVectorType;
    llvmTypeMap["vec_rational"] = rationalVectorType;
    llvmTypeMap["vec_bool"] = boolVectorType;
 
    std::vector<llvm::Type*> argTypesOneInt(1, intType);
    llvm::FunctionType* ft = llvm::FunctionType::get(intType, argTypesOneInt, false);
    llvm::Function* f  = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, MAKE_LLVM_EXTERNAL_NAME(egeszKiir), getModule());
    llvm::Function::arg_iterator i = f->arg_begin();
    if (i != f->arg_end())
        i->setName("val");
    builtins.push_back({f, (void*)egeszKiir});
 
    std::vector<llvm::Type*> argTypesOneBoolean(1, boolType);
    ft = llvm::FunctionType::get(intType, argTypesOneBoolean, false);
    f  = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, MAKE_LLVM_EXTERNAL_NAME(logikaiKiir), getModule());
    i  = f->arg_begin();
    if (i != f->arg_end())
        i->setName("val");
    builtins.push_back({f, (void*)logikaiKiir});
 
    std::vector<llvm::Type*> argTypesOneDouble(1, llvm::Type::getDoubleTy(getGlobalContext()));
    ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(getGlobalContext()), argTypesOneDouble, false);
    f  = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, MAKE_LLVM_EXTERNAL_NAME(racionalisKiir), getModule());
    i  = f->arg_begin();
    if (i != f->arg_end())
        i->setName("val");
    builtins.push_back({f, (void*)racionalisKiir});
 
    std::vector<llvm::Type*> argTypesInt8Ptr(1, llvm::Type::getInt8PtrTy(getGlobalContext()));
 
    ft = llvm::FunctionType::get(llvm::Type::getVoidTy(getGlobalContext()), argTypesInt8Ptr, true);
    f  = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, MAKE_LLVM_EXTERNAL_NAME(kiir), getModule());
    i  = f->arg_begin();
    if (i != f->arg_end())
        i->setName("str");
    builtins.push_back({f, (void*)kiir});
 
    ft = llvm::FunctionType::get(llvm::Type::getVoidTy(getGlobalContext()), argTypesInt8Ptr, true);
    f  = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, MAKE_LLVM_EXTERNAL_NAME(sorKiir), getModule());
    i  = f->arg_begin();
    if (i != f->arg_end())
        i->setName("str");
    builtins.push_back({f, (void*)sorKiir});
 
    ft = llvm::FunctionType::get(llvm::Type::getVoidTy(getGlobalContext()), true);
    f  = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, MAKE_LLVM_EXTERNAL_NAME(ujsor), getModule());
    builtins.push_back({f, (void*)ujsor});
 
    ft = llvm::FunctionType::get(llvm::Type::getVoidTy(getGlobalContext()), argTypesInt8Ptr, true);
    f  = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, MAKE_LLVM_EXTERNAL_NAME(runtimeError), getModule());
    i  = f->arg_begin();
    if (i != f->arg_end())
        i->setName("str");
    builtins.push_back({f, (void*)runtimeError});
}

bool CodeGenContext::generateCode(Block& root)
{
    outs << "Generating code...\n";
 
    std::vector<llvm::Type*> argTypes;
    llvm::FunctionType* ftype = llvm::FunctionType::get(llvm::Type::getInt32Ty(getGlobalContext()), argTypes, false);
    mainFunction = llvm::Function::Create(ftype, llvm::GlobalValue::LinkageTypes::ExternalLinkage, "main", getModule());
    llvm::BasicBlock* bblock = llvm::BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);

    llvm::DIFile* Unit = getDbgInfo()->getUnit();
    llvm::DIScope* FContext = Unit;
    unsigned LineNo = 0;
    unsigned ScopeLine = LineNo;
    llvm::DISubprogram *SP = DBuilder->createFunction(
         FContext, bblock->getName(), llvm::StringRef(), Unit, LineNo,
                getDbgInfo()->createFunctionType(llvm::Type::getInt32Ty(getGlobalContext()), argTypes), ScopeLine,
         llvm::DINode::FlagPrototyped, llvm::DISubprogram::SPFlagDefinition);
    mainFunction->setSubprogram(SP);

    newScope(bblock, SP);

    getDbgInfo()->emitLocation(&root); // debug root

    root.codeGen(*this); /* emit byte code for the top level block */

    if (errors)
    {
        outs << "Compilation error(s). Abort.\n";

#if !defined(LLVM_NO_DUMP)
        module->dump();
#endif
        return false;
    }
 
    if (currentBlock()->getTerminator() == nullptr)
    {
        auto const_int32_0 = llvm::ConstantInt::get(llvm::Type::getInt32Ty(getGlobalContext()), llvm::APInt(32, 0));
        llvm::ReturnInst::Create(getGlobalContext(), const_int32_0, currentBlock());
    }

    endScope();
 
    outs << "Code is generated.\n";
 
    //mainFunction->viewCFG();
 
    outs << "verifying... ";

    // finalize debug infos, TODO here or after verifying?
    DBuilder->finalize();

    if (verifyModule(*getModule(), &llvm::outs()))
    {
        outs << "Error constructing function!\n";
#if !defined(LLVM_NO_DUMP)
        module->dump();
#endif
        return false;
    }

    outs << "done.\n";
 
 #if !defined(LLVM_NO_DUMP) 
    module->dump();
 #endif

    return true;
}

llvm::GenericValue CodeGenContext::runCode()
{
    outs << "Running code...\n";
    std::string err;
    llvm::ExecutionEngine* ee = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(module)).setErrorStr(&err).setEngineKind(llvm::EngineKind::JIT).create();
    assert(ee);
 
    for (auto info : builtins)
    {
        ee->addGlobalMapping(info.f, info.addr);
    }
 
    ee->finalizeObject();
    std::vector<llvm::GenericValue> noargs;
    llvm::GenericValue v = ee->runFunction(mainFunction, noargs);
    outs << "Code was run.\n";
    delete ee;
    return v;
}

void CodeGenContext::printCodeGeneration(class Block& root, std::ostream& outstream)
{
    VisitorPrintAST visitor(outstream);
    root.accept(visitor);
}

void CodeGenContext::generateRuntimeError(const location loc, const std::string& str)
{
    FunctionCall error(new Identifier("runtimeError", loc), new String(str, loc), loc);
    error.codeGen(*this);
}

void CodeGenContext::optimize()
{
    outs << "Optimize code...\n";
    llvm::legacy::FunctionPassManager fpm(getModule());
    llvm::PassManagerBuilder          builder;
    builder.OptLevel = 3;
    builder.populateFunctionPassManager(fpm);
    for (auto& fn : getModule()->getFunctionList()) {
        fpm.run(fn);
    }
    fpm.run(*mainFunction);
}

void CodeGenContext::newScope(llvm::BasicBlock* bb, llvm::DIScope* ds, ScopeType scopeType)
{
    currentScopeType = scopeType;
 
    if (bb == nullptr)
    {
        bb = llvm::BasicBlock::Create(getGlobalContext(), "scope");
    }

    if (ds == nullptr)
    {
        ds = currentDebugScope();
    }
 
    codeBlocks.push_front(new CodeGenBlock(bb, ds));
}

void CodeGenContext::endScope()
{
    CodeGenBlock* top = codeBlocks.front();
    codeBlocks.pop_front();
    delete top;
    currentScopeType = ScopeType::CodeBlock;
}

llvm::AllocaInst* CodeGenContext::findVariable(std::string varName)
{
    if (currentScopeType == ScopeType::FunctionDeclaration)
    {
        auto& names = locals();
        if (names.find(varName) != names.end())
        {
            return names[varName];
        }
  
        return nullptr;
    }
 
    for (auto& cb : codeBlocks)
    {
        auto& names = cb->getValueNames();
        if (names.find(varName) != names.end())
        {
            return names[varName];
        }
    }
 
    return nullptr;
}

llvm::Type *CodeGenContext::findVectorType(std::string vecName)
{
    return nullptr;
}

void CodeGenContext::deleteVariable(std::string varName)
{
    ValueNames& names = locals();
    auto& typeMap = codeBlocks.front()->getTypeMap();
    if (names.find(varName) != names.end())
    {
        names.erase(varName);
        if (typeMap.count(varName))
        {
            typeMap.erase(varName);
        }
    }
}

void CodeGenContext::renameVariable(std::string oldVarName, std::string newVarName)
{
   ValueNames& names = locals();
   if (names.find(oldVarName) != names.end())
   {
       auto value = names[oldVarName];
       names.erase(oldVarName);
       names[newVarName] = value;
       auto& typeMap     = codeBlocks.front()->getTypeMap();
       if (typeMap.count(oldVarName))
       {
            auto val = typeMap[oldVarName];
            typeMap.erase(oldVarName);
            typeMap[newVarName] = val;
       }
   }
}

std::string CodeGenContext::getType(std::string varName)
{
   for (auto& cb : codeBlocks)
   {
       auto iter = cb->getTypeMap().find(varName);
       if (iter != std::end(cb->getTypeMap()))
       {
            return cb->getTypeMap()[varName];
       }
   }

   return std::string("");
}

llvm::Type* CodeGenContext::typeOf(const Identifier& type) { return typeOf(type.getName()); }

llvm::Type* CodeGenContext::typeOf(const std::string& name)
{
    if (llvmTypeMap.count(name) != 0)
    {
        return llvmTypeMap[name];
    }

    return voidType;
}

std::string CodeGenContext::typeNameOf(llvm::Type* type)
{
    switch( type->getTypeID() )
    {
    case llvm::Type::TypeID::DoubleTyID:
        return "rational";
    case llvm::Type::TypeID::IntegerTyID:
        if( type == llvm::Type::getInt1Ty(getGlobalContext()) )
            return "boolean";
       return "integer";
    case llvm::Type::TypeID::VoidTyID:
        return "void";
    case llvm::Type::TypeID::PointerTyID:
        return "vec_" + typeNameOf(type->getContainedType(0));
    default:
        return "void";
    }
}

llvm::Type* CodeGenContext::getGenericIntegerType()
{
#if defined(UseInt64)
    return Type::getInt64Ty(getGlobalContext());
#else
    return llvm::Type::getInt32Ty(getGlobalContext());
#endif
}

bool CodeGenContext::preProcessing(Block& root)
{
    VisitorSyntaxCheck visitor;
    root.accept(visitor);
    return !visitor.hasErrors();
}

//llvm::Value* getPointerSize(llvm::Value* pointer)
//{
//    //%Size = getelementptr %T* null, i32 1
//    //%SizeI = ptrtoint %T* %Size to i32
//    llvm::Value* sizePtr = Builder->CreateGEP(value->getType()->getContainedType(0),
//            Builder->CreateIntToPtr(llvm::ConstantInt::get(Builder->getInt32Ty(), 0), value->getType()),
//            llvm::ConstantInt::get(Builder->getInt32Ty(), 1));
//}

DebugInfo::DebugInfo(const std::string& filepath, CodeGenContext& context) : context(context)
{
    std::filesystem::path p(filepath);

    auto dir = p.parent_path().string();
    auto filename = p.filename().string();

    TheCU = context.DBuilder->createCompileUnit(
        llvm::dwarf::DW_LANG_C, context.DBuilder->createFile(filename, dir),
        "Pseudo Compiler", false, "", 0);

    Unit = context.DBuilder->createFile(TheCU->getFilename(),
                                         TheCU->getDirectory());

    auto DBuilder = context.getDBuilder();

    intType = DBuilder->createBasicType("integer", context.intType->getScalarSizeInBits(), llvm::dwarf::DW_ATE_signed);
    rationalType = DBuilder->createBasicType("rational", context.rationalType->getScalarSizeInBits(), llvm::dwarf::DW_ATE_float);
    stringType = DBuilder ->createStringType("string", context.stringType->getScalarSizeInBits());
    boolType = DBuilder->createBasicType("boolean", context.boolType->getScalarSizeInBits(), llvm::dwarf::DW_ATE_unsigned_char);
    voidType = DBuilder->createBasicType("void", context.voidType->getScalarSizeInBits(), llvm::dwarf::TypeKind::DW_ATE_unsigned);
    intVectorType = DBuilder->createPointerType(intType, intType->getSizeInBits(), intType->getAlignInBits());
    rationalVectorType = DBuilder->createPointerType(rationalType, rationalType->getSizeInBits(), rationalType->getAlignInBits());
    boolVectorType = DBuilder->createPointerType(boolType, boolType->getSizeInBits(), boolType->getAlignInBits());

    debugTypeMap["integer"] = getIntegerType();
    debugTypeMap["rational"] = getRationalType();
    debugTypeMap["string"] = getStringType();
    debugTypeMap["boolean"] = getBoolType();
    debugTypeMap["void"] = getVoidType();
    debugTypeMap["vec_integer"] = getIntegerVectorType();
    debugTypeMap["vec_rational"] = getRationalVectorType();
    debugTypeMap["vec_boolean"] = getBoolVectorType();
}

llvm::DIType* DebugInfo::debugTypeOf(llvm::Type* ty)
{
    return debugTypeMap[context.typeNameOf(ty)];
}

llvm::DIType* DebugInfo::getVoidType() {
    return voidType;
}

llvm::DIType* DebugInfo::getIntegerType() {
    return intType;
}

llvm::DIType* DebugInfo::getRationalType()
{
    return rationalType;
}

llvm::DIType* DebugInfo::getStringType()
{
    return stringType;
}

llvm::DIType* DebugInfo::getBoolType()
{
    return boolType;
}

llvm::DIType* DebugInfo::getIntegerVectorType()
{
    return intVectorType;
}

llvm::DIType* DebugInfo::getRationalVectorType()
{
    return rationalVectorType;
}

llvm::DIType* DebugInfo::getBoolVectorType()
{
    return boolVectorType;
}

llvm::DIFile* DebugInfo::getUnit()
{
    return Unit;
}

}
