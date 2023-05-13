#ifndef H_VISITOR
#define H_VISITOR

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
    class CompareOperator;
    class String;
    class VectorAddElement;

class Visitor
{
public:
    virtual void visitStatement(Statement* node) = 0;
    virtual void visitExpression(Expression* node) = 0;
    virtual void visitInteger(Integer* node) = 0;
    virtual void visitRational(Rational* node) = 0;
    virtual void visitBoolean(Boolean* node) = 0;
    virtual void visitIdentifier(Identifier* node) = 0;
    virtual void visitFunctionCall(FunctionCall* node) = 0;
    virtual void visitUnaryOperator(UnaryOperator* node) = 0;
    virtual void visitBinaryOperator(BinaryOperator* node) = 0;
    virtual void visitAssignment(Assignment* node) = 0;
    virtual void visitBlock(Block* node) = 0;
    virtual void visitVector(Vector* node) = 0;
    virtual void visitVectorAccess(VectorAccess* node) = 0;
    virtual void visitReturn(Return* node) = 0;
    virtual void visitExpressionStatement(ExpressionStatement* node) = 0;
    virtual void visitVariableDeclaration(VariableDeclaration* node) = 0;
    virtual void visitFunctionDeclaration(FunctionDeclaration* node) = 0;
    virtual void visitConditional(Conditional* node) = 0;
    virtual void visitModifyVector(ModifyVector* node) = 0;
    virtual void visitWhileLoop(WhileLoop* node) = 0;
    virtual void visitDoWhileLoop(DoWhileLoop* node) = 0;
    virtual void visitForLoop(ForLoop* node) = 0;
    virtual void visitCompareOperator(CompareOperator* node) = 0;
    virtual void visitString(String* node) = 0;
    virtual void visitVectorAddElement(VectorAddElement* node) = 0;
};
}
#endif
