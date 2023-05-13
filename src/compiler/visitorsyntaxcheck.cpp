#include "visitorsyntaxcheck.h"

#include "node.h"

namespace Pseudo {

VisitorSyntaxCheck::VisitorSyntaxCheck() {}

bool VisitorSyntaxCheck::hasErrors()
{
    return errors;
}

void VisitorSyntaxCheck::visitStatement(Statement* node) 
{

}

void VisitorSyntaxCheck::visitExpression(Expression* node)
{

}

void VisitorSyntaxCheck::visitInteger(Integer* node)
{

}

void VisitorSyntaxCheck::visitRational(Rational* node)
{

}

void VisitorSyntaxCheck::visitBoolean(Boolean* node)
{

}

void VisitorSyntaxCheck::visitIdentifier(Identifier* node)
{

}

void VisitorSyntaxCheck::visitFunctionCall(FunctionCall* node)
{

}

void VisitorSyntaxCheck::visitUnaryOperator(UnaryOperator* node)
{

}

void VisitorSyntaxCheck::visitBinaryOperator(BinaryOperator* node)
{

}

void VisitorSyntaxCheck::visitAssignment(Assignment* node)
{

}

void VisitorSyntaxCheck::visitBlock(Block* node)
{

}

void VisitorSyntaxCheck::visitVector(Vector* node)
{
}

void VisitorSyntaxCheck::visitVectorAccess(VectorAccess* node)
{

}

void VisitorSyntaxCheck::visitReturn(Return* node)
{

}

void VisitorSyntaxCheck::visitExpressionStatement(ExpressionStatement* node)
{

}

void VisitorSyntaxCheck::visitVariableDeclaration(VariableDeclaration* node)
{

}

void VisitorSyntaxCheck::visitFunctionDeclaration(FunctionDeclaration* node)
{

}

void VisitorSyntaxCheck::visitConditional(Conditional* node)
{

}

void VisitorSyntaxCheck::visitModifyVector(ModifyVector* node)
{

}

void VisitorSyntaxCheck::visitWhileLoop(WhileLoop* node)
{

}

void VisitorSyntaxCheck::visitDoWhileLoop(DoWhileLoop* node)
{

}

void VisitorSyntaxCheck::visitForLoop(ForLoop* node)
{

}

void VisitorSyntaxCheck::visitCompareOperator(CompareOperator* node)
{
}

void VisitorSyntaxCheck::visitString(String* node)
{
}

void VisitorSyntaxCheck::visitVectorAddElement(VectorAddElement* node)
{
}

}
