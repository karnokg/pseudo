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
    int value;

    Integer(int value) : value(value)
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

class Identifier : public Expression
{
public:
    std::string name;
    Identifier(const std::string& name) : name(name)
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

class Assignment : public Expression
{
public:
    Identifier* id;
    Expression* expr;

    Assignment(Identifier* id, Expression* expr) 
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

}
#endif