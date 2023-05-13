#include "string.h"

namespace Pseudo
{

String::String(const std::string& value, location loc) : value(value.substr(1, value.size() - 2))
{ 
    this->loc = loc;
}

llvm::Value* String::codeGen(CodeGenContext &context)
{
    context.getDbgInfo()->emitLocation(this);

    llvm::ArrayType* ArrayTy_0 = llvm::ArrayType::get(llvm::IntegerType::get(context.getGlobalContext(), 8), value.size() + 1);
    llvm::GlobalVariable* gvar_array__str = new llvm::GlobalVariable(*context.getModule(),
                                                         /*Type=*/ArrayTy_0,
                                                         /*isConstant=*/true,
                                                         llvm::GlobalValue::PrivateLinkage,
                                                         /*Initializer=*/0, // has initializer, specified below
                                                         ".str");
    gvar_array__str->setAlignment(llvm::MaybeAlign(1));
    llvm::Constant* const_array_str = llvm::ConstantDataArray::getString(context.getGlobalContext(), value);
    gvar_array__str->setInitializer(const_array_str);
    std::vector<llvm::Constant*> const_ptr_8_indices;
    llvm::ConstantInt* const_int = llvm::ConstantInt::get(context.getGlobalContext(), llvm::APInt(64, llvm::StringRef("0"), 10));
    const_ptr_8_indices.push_back(const_int);
    const_ptr_8_indices.push_back(const_int);
    llvm::Constant* const_ptr_8 = llvm::ConstantExpr::getGetElementPtr(ArrayTy_0, gvar_array__str, const_ptr_8_indices);
    return const_ptr_8;
}
}
