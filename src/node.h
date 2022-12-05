#ifndef H_NODE
#define H_NODE

#include <string>
#include <vector>
#include <memory>

class NStatement;
class NExpression;
class NVariableDeclarationStatement;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclarationStatement*> VariableList;

class Node 
{
    public:
        virtual ~Node() {}
        // virtual llvm::Value* codeGen(CodeGenContext& context) {}
};

class NStatement : public Node 
{

};

class NExpression : public Node 
{

};

class NInteger : public NExpression 
{
public:
    int value;

    NInteger(int value) : value(value)
    {
    }
};

class NRational : public NExpression
{
public:
    double value;

    NRational(double value) : value(value)
    {
    }
};

class NIdentifier : public NExpression
{
public:
    std::string name;
    NIdentifier(const std::string& name) : name(name)
    {
    }
};

class NFunctionCall : public NExpression
{
public:
    const NIdentifier& id;
    ExpressionList args;
    NFunctionCall(const NIdentifier& id, const ExpressionList& args) 
    : id(id), args(args)
    {
    }

    NFunctionCall(const NIdentifier& id) : id(id)
    {
    }
};

class NBinaryOperator : public NExpression
{
public:
    int op;
    const NExpression& lhs;
    const NExpression& rhs;

    NBinaryOperator(const NExpression& lhs, int op, const NExpression& rhs) 
    : op(op), lhs(lhs), rhs(rhs)
    {
    }
};

class NAssignment : public NExpression
{
public:
    const NIdentifier& id;
    const NExpression& expr;

    NAssignment(const NIdentifier& id, const NExpression& expr) 
    : id(id), expr(expr)
    {
    }
};

class NBlock : public NExpression
{
public:
    StatementList statements;
    NBlock();
};

class NExpressionStatement : public NStatement
{
public:
    const NExpression& expression;

    NExpressionStatement(const NExpression& expression) 
    : expression(expression)
    {
    }
};

class NVariableDeclarationStatement : public NStatement
{
public:
    const NIdentifier& type;
    const NIdentifier& id;
    const NExpression* assignmentExpr;

    NVariableDeclarationStatement(const NIdentifier& type, NIdentifier& id)
    : type(type), id(id)
    {
    }

    NVariableDeclarationStatement(const NIdentifier& type, NIdentifier& id,
    NExpression* assignmentExpr)
    : type(type), id(id), assignmentExpr(assignmentExpr)
    {
    }
};

class NFunctionDeclaration : public NStatement
{
public:
    const NIdentifier& type;
    const NIdentifier& id;
    VariableList args;
    StatementList statements;

    NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id,
        VariableList args, StatementList statements)
        : type(type), id(id), args(args), statements(statements)
    {
    }
};
#endif