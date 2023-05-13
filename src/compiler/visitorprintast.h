#ifndef H_VISITORPRINTAST
#define H_VISITORPRINTAST

#include <iostream>
#include "visitor.h"

namespace Pseudo
{
    class Statement;
    class Expression;
    class Integer;
    class Rational;
    class Boolean;
    class Identifier;
    class FunctionCall;
    class UnaryOperator;
    class BinaryOperator;
    class Assignment;
    class Block;
    class Vector;
    class VectorAccess;
    class Return;
    class ExpressionStatement;
    class VariableDeclaration;
    class FunctionDeclaration;
    class Conditional;
    class ModifyVector;
    class WhileLoop;
    class DoWhileLoop;
    class ForLoop;
    class String;
    class VectorAddElement;

class VisitorPrintAST : public Visitor
{
    int currIndent{0};
    std::ostream& out;
public:
    VisitorPrintAST(std::ostream& outs);
    void visitStatement(Statement* node) override;
    void visitExpression(Expression* node) override;
    void visitInteger(Integer* node) override;
    void visitRational(Rational* node) override;
    void visitBoolean(Boolean* node) override;
    void visitIdentifier(Identifier* node) override;
    void visitFunctionCall(FunctionCall* node) override;
    void visitUnaryOperator(UnaryOperator* node) override;
    void visitBinaryOperator(BinaryOperator* node) override;
    void visitAssignment(Assignment* node) override;
    void visitBlock(Block* node) override;
    void visitVector(Vector* node) override;
    void visitVectorAccess(VectorAccess* node) override;
    void visitReturn(Return* node) override;
    void visitExpressionStatement(ExpressionStatement* node) override;
    void visitVariableDeclaration(VariableDeclaration* node) override;
    void visitFunctionDeclaration(FunctionDeclaration* node) override;
    void visitConditional(Conditional* node) override;
    void visitModifyVector(ModifyVector* node) override;
    void visitWhileLoop(WhileLoop* node) override;
    void visitDoWhileLoop(DoWhileLoop* node) override;
    void visitForLoop(ForLoop* node) override;
    void visitCompareOperator(CompareOperator* node) override;
    void visitString(String* node) override;
    void visitVectorAddElement(VectorAddElement* node);
};
}
#endif
