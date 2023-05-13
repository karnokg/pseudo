#ifndef H_PSEUDO
#define H_PSEUDO

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <map>

extern llvm::LLVMContext context;
extern llvm::IRBuilder<> builder;
extern std::unique_ptr<llvm::Module> module;
extern std::map<std::string, llvm::Value*> namedValues;

#endif