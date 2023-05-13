#include "visitorprintast.h"
#include "node.h"
#include "nodes/binaryoperator.h"
#include "nodes/unaryoperator.h"
#include "nodes/functiondeclaration.h"
#include "nodes/variabledeclaration.h"
#include "nodes/compareoperator.h"
#include "nodes/assignment.h"
#include "nodes/string.h"
#include "nodes/vector.h"
#include "nodes/vectoraddelement.h"
#include "nodes/forloop.h"

namespace Pseudo
{

static inline std::string indent(int indent)
{
   return std::string(indent * 4, ' ');
}

VisitorPrintAST::VisitorPrintAST(std::ostream &outs) : out(outs) {}

void VisitorPrintAST::visitStatement(Statement* node)
{
    out << indent(currIndent) << node->str() << std::endl;
}

void VisitorPrintAST::visitExpression(Expression* node)
{
    out << indent(currIndent) << node->str() << std::endl;
}

void VisitorPrintAST::visitInteger(Integer* node)
{
    out << indent(currIndent) << node->str() << std::endl;
}

void VisitorPrintAST::visitRational(Rational* node)
{
    out << indent(currIndent) << node->str() << std::endl;
}

void VisitorPrintAST::visitBoolean(Boolean* node)
{
    out << indent(currIndent) << node->str() << std::endl;
}

void VisitorPrintAST::visitIdentifier(Identifier* node)
{
    out << indent(currIndent) << node->str() << std::endl;
}

void VisitorPrintAST::visitFunctionCall(FunctionCall* node)
{
    out << indent(currIndent) << node->str() << std::endl;

    ++currIndent;

    auto args = node->getArguments();
    if(!args->empty()) 
    {
        out << indent(currIndent) << "Arguments" << std::endl;
        for(auto arg : *args) 
        {
            arg->accept(*this);
        }
    }

    --currIndent;
}

void VisitorPrintAST::visitUnaryOperator(UnaryOperator* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    ++currIndent;
    node->getRHS()->accept(*this);
    --currIndent;
}

void VisitorPrintAST::visitBinaryOperator(BinaryOperator* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    ++currIndent;
    node->getLHS()->accept(*this);
    node->getRHS()->accept(*this);
    --currIndent;
}

void VisitorPrintAST::visitAssignment(Assignment* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    ++currIndent;
    node->getExpression()->accept(*this);
    --currIndent;
}

void VisitorPrintAST::visitBlock(Block* node)
{
    out << indent(currIndent) << "BEGIN" << node->str() << std::endl;
    ++currIndent;

    for(auto stmt : node->statements)
    {
       stmt->accept(*this);
    }

    --currIndent;
    out << indent(currIndent) << "END" << node->str() << std::endl;
}

void VisitorPrintAST::visitVector(Vector* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    ++currIndent;
    for (auto e : *node->getExpressions())
    {
        e->accept(*this);
    }
    --currIndent;
}

void VisitorPrintAST::visitVectorAccess(VectorAccess* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    ++currIndent;
    for (auto e : *node->getIndexExpressions())
    {
        e->accept(*this);
    }
    --currIndent;
}

void VisitorPrintAST::visitReturn(Return* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    if (node->hasRetExpression())
    {
        ++currIndent;
        node->getRetExpression()->accept(*this);
        --currIndent;
    }
}

void VisitorPrintAST::visitExpressionStatement(ExpressionStatement* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    ++currIndent;
    node->getExpression()->accept(*this);
    --currIndent;
}

void VisitorPrintAST::visitVariableDeclaration(VariableDeclaration* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    if (node->hasAssignmentExpr())
    {
        ++currIndent;
        node->getAssignment()->accept(*this);
        --currIndent;
    }
}

void VisitorPrintAST::visitFunctionDeclaration(FunctionDeclaration* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    auto params = node->getParameters();
    ++currIndent;

    if(!params->empty()) 
    {
       out << indent(currIndent) << "[" << std::endl;
       ++currIndent;
       out << indent(currIndent) << "Parameters :" << std::endl;
       for (auto decl : *params)
       {
          out << indent(currIndent) << "*" << decl->getVariableTypeName() << " " << decl->getVariableName() << std::endl;
       }
       out << indent(currIndent) << " ]" << std::endl;
       --currIndent;
    }

   auto body = node->getBody();
   for (auto stmt : body->statements) 
   {
      stmt->accept(*this);
   }

   --currIndent;
}

void VisitorPrintAST::visitConditional(Conditional* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    ++currIndent;

    node->getCondExpression()->accept(*this);

    if (node->getThenExpression())
    {
       node->getThenExpression()->accept(*this);
    }
    if (node->getElseExpression())
    {
       node->getElseExpression()->accept(*this);
    }

    --currIndent;
}

void VisitorPrintAST::visitModifyVector(ModifyVector* node)
{
    std::cout << indent(currIndent) << node->str() << std::endl;
    out << indent(currIndent) << "[" << std::endl;
    ++currIndent;
    out << indent(currIndent) << "Indexes" << std::endl;
    for (auto e : *node->getIndexExpressions())
    {
        e->accept(*this);
    }

    out << indent(currIndent) << "New Value" << std::endl;
    node->getNewValue()->accept(*this);
    --currIndent;
    out << indent(currIndent) << "]" << std::endl;
}

void VisitorPrintAST::visitWhileLoop(WhileLoop* node)
{
   out << indent(currIndent) << node->str() << std::endl;
   ++currIndent;
   node->getCondition()->accept(*this);
   out << indent(currIndent) << "LOOP BODY BEGIN" << std::endl;
   node->getLoopBlock()->accept(*this);
   out << indent(currIndent) << "LOOP BODY END" << std::endl;
   --currIndent;
}

void VisitorPrintAST::visitDoWhileLoop(DoWhileLoop* node)
{
   out << indent(currIndent) << node->str() << std::endl;
   ++currIndent;
   node->getCondition()->accept(*this);
   out << indent(currIndent) << "LOOP BODY BEGIN" << std::endl;
   node->getLoopBlock()->accept(*this);
   out << indent(currIndent) << "LOOP BODY END" << std::endl;
   --currIndent;
}

void VisitorPrintAST::visitForLoop(ForLoop* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    ++currIndent;
    out << indent(currIndent) << "FROM" << std::endl;
    node->getFromExpr()->accept(*this);
    out << indent(currIndent) << "TO" << std::endl;
    node->getToExpr()->accept(*this);
    out << indent(currIndent) << "LOOP BEGIN" << std::endl;
    node->getLoopBlock()->accept(*this);
    out << indent(currIndent) << "LOOP END" << std::endl;
    --currIndent;
}

void VisitorPrintAST::visitCompareOperator(CompareOperator* node)
{
    out << indent(currIndent) << node->str() << std::endl;
    ++currIndent;
    node->getLHS()->accept(*this);
    node->getRHS()->accept(*this);
    --currIndent;
}

void VisitorPrintAST::visitString(String* node)
{
    out << indent(currIndent) << node->str() << std::endl;
}

void VisitorPrintAST::visitVectorAddElement(VectorAddElement* node)
{
    out << "TODO";
}

}
