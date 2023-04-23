#ifndef H_NODE
#define H_NODE

#include <string>
#include <vector>
#include <memory>

namespace Pseudo {

class Statement;
class Expression;
class VariableDeclarationStatement;

typedef std::vector<Statement*> StatementList;
typedef std::vector<Expression*> ExpressionList;
typedef std::vector<VariableDeclarationStatement*> VariableList;

class Node 
{
public:
    virtual ~Node() {}
    //virtual llvm::Value* codeGen(CodeGenContext& context) = 0;
};

class Statement : public Node 
{

};

class Expression : public Node 
{

};

class Integer : public Expression 
{
public:
    long long value;

    Integer(long long value) : value(value)
    {
    }
};

class Rational : public Expression
{
public:
    double value;

    Rational(double value) : value(value)
    {
    }
};

class Boolean : public Expression
{
public:
    bool value;

    Boolean(bool value) : value(value)
    {
    }
};

class Identifier : public Expression
{
public:
    std::string name;
    Identifier(std::string name) : name(name)
    {
    }
};

class FunctionCall : public Expression
{
public:
    Identifier* id;
    ExpressionList* args;
    FunctionCall(Identifier* id, ExpressionList* args) 
    : id(id), args(args)
    {
    }

    FunctionCall(Identifier* id) : id(id)
    {
    }
};

class UnaryOperator : public Expression
{
public:
    int op;
    Expression* rhs;

    UnaryOperator(int op, Expression* rhs) 
    : op(op), rhs(rhs)
    {
    }
};

class BinaryOperator : public Expression
{
public:
    int op;
    Expression* lhs;
    Expression* rhs;

    BinaryOperator(Expression* lhs, int op, Expression* rhs) 
    : op(op), lhs(lhs), rhs(rhs)
    {
    }
};

class AssignmentStatement : public Statement
{
public:
    Identifier* id;
    Expression* expr;

    AssignmentStatement(Identifier* id, Expression* expr) 
    : id(id), expr(expr)
    {
    }
};

class Block : public Expression
{
public:
    StatementList statements;
    Block() {}
};

class Vector : public Expression
{
public:
    Identifier* type;
    ExpressionList* exprs;

    int maxSize;

    //Vector(Identifier* id) : id(id) {}
    Vector(Identifier* type, ExpressionList* exprs, const int maxSize) 
        : type(type), exprs(exprs), maxSize(maxSize) {}
};

class VectorAccess : public Expression
{
public:
    Identifier* id;
    ExpressionList* indexes; 
    VectorAccess(Identifier* id, Expression* expr) : id(id) 
    {
        indexes = new ExpressionList();
        indexes->push_back(expr);
    }

    VectorAccess(Identifier* id, ExpressionList* indexes) 
        : id(id), indexes(indexes) 
    {
    }
};

class Array : public Expression
{
    ExpressionList exprs;
    Array()
    {
    }

    Array(ExpressionList& exprs) : exprs(exprs)
    {
    } 
};

class ArrayAccess : public Expression
{
};

class ArrayAddElement : public Expression
{
};

class Return : public Statement 
{
    Expression* expr;

    Return(Expression* expr)
    : expr(expr)
    {
    }
};

class ExpressionStatement : public Statement
{
public:
    Expression* expression;

    ExpressionStatement(Expression* expression) 
    : expression(expression)
    {
    }
};

class VariableDeclarationStatement : public Statement
{
public:
    Identifier* type;
    Identifier* id;
    Expression* assignmentExpr;

    VariableDeclarationStatement(Identifier* type, Identifier* id)
    : type(type), id(id)
    {
    }

    VariableDeclarationStatement(Identifier* type, Identifier* id,
    Expression* assignmentExpr)
    : type(type), id(id), assignmentExpr(assignmentExpr)
    {
    }
};

class FunctionDeclaration : public Statement
{
public:
    const Identifier* type;
    const Identifier* id;
    VariableList* args;
    Block* block;

    FunctionDeclaration(Identifier* type, Identifier* id,
        VariableList* args, Block* block)
        : type(type), id(id), args(args), block(block)
    {
    }
};

class ConditionalStatement : public Statement
{
public:
    const Expression* boolExpr;
    const Block* thenBlock;
    const Block* elseBlock;

    ConditionalStatement(Expression* condition, Block* thenBlock,
        Block* elseBlock = nullptr) : boolExpr(condition),
        thenBlock(thenBlock), elseBlock(elseBlock) 
    {
    }
};

class ModifyVector : public Statement
{
public:
    Identifier* id;
    ExpressionList* indexes; 
    Expression* newval;

    ModifyVector(VectorAccess* access, Expression* newval)
        : id(access->id), indexes(access->indexes), newval(newval)
    {

    }

    ModifyVector(Identifier* id, ExpressionList* indexes, Expression* newval)
        : id(id), indexes(indexes), newval(newval)
    {
    }
};

}
#endif