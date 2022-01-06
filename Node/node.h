#pragma once
#include <iostream>
#include "llvm/IR/Value.h"
#include "CodeGenContext.h"

class Node {
public:
    virtual ~Node() { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};