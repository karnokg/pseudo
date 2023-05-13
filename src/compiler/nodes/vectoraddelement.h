#ifndef H_VECTORADDELEMENT
#define H_VECTORADDELEMENT

#include "node.h"
#include "location.hh"
#include "visitor.h"

// TODO not tested and not constructed from parser!

namespace Pseudo
{

class VectorAddElement : public Statement
{
private:
   Expression* expr{nullptr};
   Identifier* ident{nullptr};

public:
   VectorAddElement(Identifier* ident, Expression* expr, location loc) : ident(ident), expr(expr) { this->loc = loc; }

   llvm::Value* codeGen(CodeGenContext& context) override;
   NodeType getType() override { return NodeType::Vector; }
   std::string str() override { return "[ VectorAddElement ]"; }
   void accept(Visitor& v) override { v.visitVectorAddElement(this); }
   Expression* getExpression() const { return expr; }
};

}

#endif