#include <string>
#include <vector>
#include <memory>

typedef std::vector<std::shared_ptr<NStatement>> StatementList;
typedef std::vector<std::shared_ptr<NExpression>> ExpressionList;
typedef std::vector<std::shared_ptr<NVariableDeclarationStatement>> VariableList;

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
    int m_value;

    NInteger(int value) : m_value(value)
    {
    }
};

class NRational : public NExpression
{
public:
    double m_value;

    NRational(double value) : m_value(value)
    {
    }
};

class NIdentifier : public NExpression
{
public:
    std::string m_name;
    NIdentifier(const std::string& name) : m_name(name)
    {
    }
};

class NFunctionCall : public NExpression
{
public:
    const NIdentifier& m_id;
    ExpressionList m_args;
    NFunctionCall(const NIdentifier& id, const ExpressionList& args) 
    : m_id(id), m_args(args)
    {
    }

    NFunctionCall(const NIdentifier& id) : m_id(id)
    {
    }
};

class NBinaryOperator : public NExpression
{
public:
    int m_op;
    const NExpression& m_lhs;
    const NExpression& m_rhs;

    NBinaryOperator(const NExpression& lhs, int op, const NExpression& rhs) 
    : m_op(op), m_lhs(lhs), m_rhs(rhs)
    {
    }
};

class NAssignment : public NExpression
{
public:
    const NIdentifier& m_id;
    const NExpression& m_expr;

    NAssignment(const NIdentifier& id, const NExpression& expr) 
    : m_id(id), m_expr(expr)
    {
    }
};

class NExpressionStatement : public NStatement
{
public:
    const NExpression& m_expression;

    NExpressionStatement(const NExpression& expression) 
    : m_expression(expression)
    {
    }
};

class NVariableDeclarationStatement : public NStatement
{
public:
    const NIdentifier& m_type;
    const NIdentifier& m_id;
    const std::shared_ptr<NExpression> m_assignmentExpr;

    NVariableDeclarationStatement(const NIdentifier& type, NIdentifier& id)
    : m_type(type), m_id(id)
    {
    }

    NVariableDeclarationStatement(const NIdentifier& type, NIdentifier& id,
    std::shared_ptr<NExpression> assignmentExpr)
    : m_type(type), m_id(id), m_assignmentExpr(assignmentExpr)
    {
    }
};

class NFunctionDeclaration : public NStatement
{
public:
    const NIdentifier& m_type;
    const NIdentifier& m_id;
    VariableList m_args;
    StatementList m_statements;

    NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id,
        VariableList args, StatementList statements)
        : m_type(type), m_id(id), m_args(args), m_statements(statements)
    {
    }
};