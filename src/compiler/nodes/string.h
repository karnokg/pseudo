#ifndef H_STRING
#define H_STRING

#include <string>
#include "../node.h"

namespace Pseudo
{

class String : public Expression
{
public:
    std::string value;

    String(const std::string& value, location loc);
    void accept(Visitor& v) override { v.visitString(this); }
    llvm::Value* codeGen(CodeGenContext& context) override;
    NodeType getType() override { return NodeType::String; }
    std::string str() override
    {
        return "[ String(value=" + value + ") ]";
    }
};
    
} 

#endif