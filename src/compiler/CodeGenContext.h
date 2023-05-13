#ifndef H_CODEGENCONTEXT
#define H_CODEGENCONTEXT

#include <vector>
#include <map>
#include <list>
#include <set>

#include <llvm/IR/Module.h>
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/Support/ManagedStatic.h>

#include "location.hh"

namespace Pseudo
{

class DebugInfo;

enum class ScopeType {
   FunctionDeclaration,
   CodeBlock,
};

using ValueNames = std::map<std::string, llvm::AllocaInst*>;
using VariableTypeMap = std::map<std::string, std::string>;

class CodeGenBlock
{
public:
    CodeGenBlock(llvm::BasicBlock* bb, llvm::DIScope* ds)
    {
        bblock = bb;
        debugScope = ds;
    }
    ~CodeGenBlock() {}
    void              setCodeBlock(llvm::BasicBlock* bb) { bblock = bb; }
    llvm::BasicBlock* currentBlock() { return bblock; }
    ValueNames&       getValueNames() { return locals; }
    VariableTypeMap&  getTypeMap() { return types; }
    llvm::DIScope* currentDebugScope() { return debugScope; };

private:
    llvm::BasicBlock* bblock{nullptr};
    ValueNames locals;
    VariableTypeMap types;
    llvm::DIScope* debugScope{nullptr};
};

class CodeGenContext
{
public:
    llvm::Value* varStruct{nullptr};
    bool verbose {false};           
 
    CodeGenContext(const std::string& filename, std::ostream& outs);
    ~CodeGenContext() 
    {
      llvm::llvm_shutdown(); 
    }
 
    llvm::Module* getModule() const { return module; }
 
    llvm::LLVMContext& getGlobalContext() { return llvmContext; }
 
    void newScope(llvm::BasicBlock* bb = nullptr, llvm::DIScope* ds = nullptr, ScopeType scopeType = ScopeType::CodeBlock);
 
    void endScope();
 
    ScopeType getScopeType() { return currentScopeType; }
 
    void setInsertPoint(llvm::BasicBlock* bblock) { setCurrentBlock(bblock); }
 
    llvm::BasicBlock* getInsertPoint() { return currentBlock(); }
 
    bool generateCode(class Block& root);
 
    llvm::GenericValue runCode();
 
    void printCodeGeneration(class Block & root, std::ostream & outs);
 
    void generateRuntimeError(const location loc, const std::string& str);
 
    ValueNames& locals() { return codeBlocks.front()->getValueNames(); }
 
    void setVarType(std::string varTypeName, std::string varName) { codeBlocks.front()->getTypeMap()[varName] = varTypeName; }
 
    std::string getType(std::string varName);
 
    llvm::AllocaInst* findVariable(std::string varName);
 
    llvm::Type* findVectorType(std::string vecName);
 
    void deleteVariable(std::string varName);
 
    void renameVariable(std::string oldVarName, std::string newVarName);
 
    llvm::BasicBlock* currentBlock() { return codeBlocks.front()->currentBlock(); }

    llvm::DIScope* currentDebugScope() { return codeBlocks.front()->currentDebugScope(); }
 
    void optimize();
 
    llvm::Type* typeOf(const class Identifier& type);
 
    llvm::Type* typeOf(const std::string& name);
 
    std::string typeNameOf(llvm::Type* type);
 
    llvm::Type* getGenericIntegerType();
 
    bool preProcessing(class Block & root);
 
    void addError() { ++errors; }

    DebugInfo* getDbgInfo() { return DbgInfo; }

    llvm::DIBuilder* getDBuilder() { return DBuilder; }

    llvm::IRBuilder<>* getBuilder() { Builder->SetInsertPoint(currentBlock()); return Builder; }
private:
    void setCurrentBlock(llvm::BasicBlock * block) { codeBlocks.front()->setCodeBlock(block); }
 
    void setupBuiltIns();
 
    std::list<CodeGenBlock*> codeBlocks;             

    DebugInfo* DbgInfo{nullptr};
    llvm::DIBuilder* DBuilder{nullptr};
    llvm::IRBuilder<>* Builder{nullptr};

    llvm::Function* mainFunction{nullptr};  
    llvm::Module* module{nullptr};        
    llvm::LLVMContext llvmContext;            
    int errors{0};              
    ScopeType currentScopeType{ScopeType::CodeBlock};
    std::ostream& outs;
    struct buildin_info_t
    {
       llvm::Function* f{nullptr};
       void* addr{nullptr};
    };
    std::vector<buildin_info_t> builtins;
    llvm::Type* intType {nullptr};
    llvm::Type* rationalType {nullptr};
    llvm::Type* stringType {nullptr};
    llvm::Type* boolType {nullptr};
    llvm::Type* voidType {nullptr};
 
    llvm::Type* intVectorType{nullptr};
    llvm::Type* rationalVectorType{nullptr};
    llvm::Type* boolVectorType{nullptr};
 
    std::map<std::string, llvm::Type*> llvmTypeMap;

    friend DebugInfo;
};

struct DebugInfo
{
    CodeGenContext& context;

    DebugInfo(const std::string& filename, CodeGenContext& context);

    //std::list<llvm::DIScope*> lexicalBlocks;

    llvm::DICompileUnit* TheCU;

    llvm::DIFile* Unit;
    llvm::DIType* intType;
    llvm::DIType* rationalType;
    llvm::DIType* stringType;
    llvm::DIType* boolType;
    llvm::DIType* voidType;
    llvm::DIType* intVectorType;
    llvm::DIType* rationalVectorType;
    llvm::DIType* boolVectorType;

    std::map<std::string, llvm::DIType*> debugTypeMap;

    llvm::DIType* debugTypeOf(llvm::Type* ty);

    llvm::DIType* getVoidType();

    llvm::DIType* getIntegerType();

    llvm::DIType* getRationalType();

    llvm::DIType* getStringType();

    llvm::DIType* getBoolType();

    llvm::DIType* getIntegerVectorType();

    llvm::DIType* getRationalVectorType();

    llvm::DIType* getBoolVectorType();

    llvm::DIFile* getUnit();


 //   void newSubprogram(llvm::DISubprogram* SP)
 //   {
 //       lexicalBlocks.push_front(SP);
 //   }

 //   void endSubprogram()
 //   {
 //       lexicalBlocks.pop_front();
 //   }

 //   llvm::DIScope* currentSubprogram()
 //   {
 //       return lexicalBlocks.front();
 //   }

    void emitLocation(location loc)
    {
        auto Builder = context.getBuilder();
        auto DBuilder = context.getDBuilder();

        llvm::DIScope* Scope;
        if (context.currentDebugScope() == nullptr)
        {
            Scope = TheCU;
        }
        else
        {
            Scope = context.currentDebugScope();
        }

        //std::cout << "Raw Location emitted here " << loc.begin.line << "|" << loc.begin.column << std::endl;

        auto DLocation = llvm::DILocation::get(Scope->getContext(), loc.begin.line, loc.begin.column, Scope);
        Builder->SetCurrentDebugLocation(DLocation);

    }

    template <typename T>
    void emitLocation(T* node)
    {
        auto Builder = context.getBuilder();
        auto DBuilder = context.getDBuilder();

        if (!node)
        {
            Builder->SetCurrentDebugLocation(llvm::DebugLoc());
            return;
        }

        llvm::DIScope* Scope;
        if (context.currentDebugScope() == nullptr)
        {
            Scope = TheCU;
        }
        else
        {
            Scope = context.currentDebugScope();
        }

        location loc = node->getLocation();

        //std::cout << "Location emitted here " << loc.begin.line << "|" << loc.begin.column << std::endl;

        auto DLocation = llvm::DILocation::get(Scope->getContext(), loc.begin.line, loc.begin.column, Scope);
        Builder->SetCurrentDebugLocation(DLocation);
    }

    llvm::DISubroutineType* createFunctionType(llvm::Type* retTy, std::vector<llvm::Type*> args)
    {
        auto DBuilder = context.getDBuilder();

        std::vector<llvm::Metadata*> EltTys;
//        llvm::SmallVector<llvm::Metadata*, 8> EltTys;

        EltTys.push_back(debugTypeOf(retTy)); // result type

        for (auto ty : args)
        {
            EltTys.push_back(debugTypeOf(ty));
        }

        auto tyArray = DBuilder->getOrCreateTypeArray(EltTys);
        return DBuilder->createSubroutineType(tyArray);
    }
};

}
#endif
