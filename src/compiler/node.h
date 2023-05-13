#ifndef H_NODE
#define H_NODE

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include "CodeGenContext.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Analysis/ValueTracking.h"

#include "visitor.h"
#include "location.hh"

namespace Pseudo {

class Statement;
class Expression;
class VariableDeclaration;

typedef std::vector<Statement*> StatementList;
typedef std::vector<Expression*> ExpressionList;
typedef std::vector<VariableDeclaration*> VariableList;

enum class NodeType
{
    Statement,
    Expression,
    Variable,
    Function,
    Integer,
    Rational,
    String,
    Boolean,
    Identifier,
    Vector,
    Iterator,
    Block,
};

class Node 
{
protected:
    location loc;

public:
    virtual ~Node() {}
    virtual llvm::Value* codeGen(CodeGenContext& context) = 0;
    virtual NodeType getType() = 0;
    virtual void accept(Visitor& visitor) = 0;
    virtual std::string str() = 0;

    location getLocation() { return loc; }

    static void printError(const location& location, const std::string& str);

    static void printError(const std::string& str)
    {
        std::cout << str << std::endl;
    }
};

class Statement : public Node 
{
public:
    NodeType getType() override
    {
        return NodeType::Statement;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitStatement(this);
    }

    std::string str() override
    {
        return "[ Statement ]";
    }
};

class Expression : public Node 
{
public:
    virtual NodeType getType() override
    {
        return NodeType::Expression;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitExpression(this);
    }

    std::string str() override
    {
        return "[ Expression ]";
    }
};

class Integer : public Expression 
{
public:
    long long value;
    
    Integer(long long value, location loc) : value(value) 
    {
        this->loc = loc;
    }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        context.getDbgInfo()->emitLocation(this);
        return llvm::ConstantInt::get(context.getGenericIntegerType(), value, true);
    }

    NodeType getType() override
    {
        return NodeType::Integer;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitInteger(this);
    }

    std::string str() override
    {
        return "[integer (value=" + std::to_string(value) + ")]";
    }
};

class Rational : public Expression
{
public:
    double value;

    Rational(double value, location loc) : value(value)
    {
        this->loc = loc;
    }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        context.getDbgInfo()->emitLocation(this);
        return llvm::ConstantFP::get(llvm::Type::getDoubleTy(context.getGlobalContext()), value);
    }

    NodeType getType() override
    {
        return NodeType::Rational;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitRational(this);
    }

    std::string str() override
    {
        return "[rational (value=" + std::to_string(value) + ")]";
    }
};

class Boolean : public Expression
{
public:
    bool value;

    Boolean(bool value, location loc) : value(value)
    {
        this->loc = loc;
    }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        auto Builder = context.getBuilder();
        Builder->SetInsertPoint(context.currentBlock());

        context.getDbgInfo()->emitLocation(this);
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context.getGlobalContext()), (int) value);
    }

    NodeType getType() override
    {
        return NodeType::Boolean;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitBoolean(this);
    }

    std::string str() override
    {
        return "[boolean (value=" + std::to_string(value) + ")]";
    }
};

class Identifier : public Expression
{
private:
    std::string name;

public:
    Identifier(std::string name, location loc) : name(name) 
    {
        this->loc = loc;
    }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        llvm::AllocaInst* alloc = context.findVariable(name);

        if (!alloc)
        {
            Node::printError(loc, "Nem letezik ilyen valtozo: " + name);
            context.addError();
            return nullptr;
        }

        context.getDbgInfo()->emitLocation(this);
        return  context.getBuilder()->CreateLoad(alloc->getAllocatedType(), alloc, "");
//        return new llvm::LoadInst(alloc->getAllocatedType(), alloc, "", false, context.currentBlock());
    }

    NodeType getType() override
    {
        return NodeType::Identifier;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitIdentifier(this);
    }

    std::string str() override
    {
        return "[identifier (name=" + name + ")]";
    }

    std::string getName() const
    {
        return name;
    }
};

class FunctionCall : public Expression
{
public:
    Identifier* id;
    ExpressionList* arguments;

    FunctionCall(Identifier* id, ExpressionList* arguments, location loc)
        : id(id), arguments(arguments)
    {
        this->loc = loc;
    }

    FunctionCall(Identifier* id, Expression* argument, location loc)
        : id(id)
    {
        arguments = new ExpressionList();
        arguments->push_back(argument);
        this->loc = loc;
    }

    FunctionCall(Identifier* id, location loc) : id(id)
    {
        this->loc = loc;
    }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        std::string functionName = id->getName();

        llvm::Function* function = context.getModule()->getFunction(functionName);

        if (!function)
        {
            Node::printError(loc, "Nincs ilyen fuggveny: " + functionName);
            context.addError();
            return nullptr;
        }

        std::vector<llvm::Value*> args;

        if (function->arg_size() != arguments->size())
        {
            Node::printError(loc, "A fuggveny `" + functionName + "` hibas mennyisegu parameterrel lett meghivva! Kellene: " + std::to_string(function->arg_size()) + ", de ennyi van: " + std::to_string(arguments->size()));
            context.addError();
            return nullptr;
        }

        for (size_t i = 0; i < arguments->size(); ++i)
        {
            auto expr = (*arguments)[i];
            auto arg = expr->codeGen(context);
            if (!arg)
            {
                Node::printError(loc, "A `" + functionName + "` fuggvenyhivas " + std::to_string(i) + ". parametere nem kiertekelheto!");
                context.addError();
                return nullptr;
            }

            auto expectedType = function->getArg(i)->getType();
            if (expectedType != arg->getType())
            {
                Node::printError(loc, "A `" + functionName + "` fuggvenyhivas " + std::to_string(i + 1) + ". parametere nem egyezik az elvarttal! Kellene: " + context.typeNameOf(expectedType) + ", de a kapott parameter: " + context.typeNameOf(arg->getType()));
                context.addError();
                return nullptr;
            }

            args.push_back(arg);
        }

        auto Builder = context.getBuilder();
        Builder->SetInsertPoint(context.currentBlock());

        context.getDbgInfo()->emitLocation(this);
        auto res = Builder->CreateCall(function, args, "");
        return res;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitFunctionCall(this);
    }

    NodeType getType() override
    {
        return NodeType::Expression;
    }

    ExpressionList* getArguments() 
    {
        return arguments;
    }

    std::string str()
    {
        return "[ FunctionCall (id=" + id->getName() + ")]";
    }
};


class Block : public Expression
{
public:
    StatementList statements;
    Block(StatementList statementList, location loc) : statements(statementList) { this->loc = loc; }
    Block(location loc) { this->loc = loc; }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        llvm::Value* last = nullptr;

        for (auto s : statements)
        {
            last = s->codeGen(context);
            if (!last)
            {
                Node::printError("Blokk kiertekelese nem sikerult!");
                context.addError();
                return nullptr;
            }
        }

        return last;
    }

    NodeType getType() override
    {
        return NodeType::Block;
    }

    std::string str() override
    {
        return "[ block ]";
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitBlock(this);
    }
};


class VectorAccess : public Expression
{
public:
    Identifier* id;
    ExpressionList* indexExprs;

    VectorAccess(Identifier* id, Expression* expr, location loc) : id(id)
    {
        indexExprs = new ExpressionList();
        indexExprs->push_back(expr);
        this->loc = loc;
    }

    VectorAccess(Identifier* id, ExpressionList* indexExprs, location loc)
        : id(id), indexExprs(indexExprs)
    {
        this->loc = loc;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitVectorAccess(this);
    }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        context.getDbgInfo()->emitLocation(this);
        llvm::IRBuilder<> builder(context.getGlobalContext());
        builder.SetInsertPoint(context.currentBlock());

        llvm::AllocaInst* var = context.findVariable(id->getName());

        if (!var)
        {
            Node::printError(loc, "unknown variable " + id->getName());
            context.addError();
            return nullptr;
        }

        auto var_type = var->getAllocatedType();
        auto var_struct_type = var->getAllocatedType()->getContainedType(0);
        auto test_type = var->getType()->getScalarType();

        if (!var_struct_type)
        {
            Node::printError(loc, "Variable " + id->getName() + " nem vector,, hanem " + context.getType(id->getName()));
            context.addError();
            return nullptr;
        }

        if (var_type->getTypeID() != llvm::StructType::PointerTyID)
        {
            Node::printError(loc, "Variable " + id->getName() + " tipusa nem egyezik a vektorokkal, hanem " + context.getType(id->getName()));
            context.addError();
            return nullptr;
        }

        if (indexExprs->empty())
        {
            Node::printError(loc, "Nincs index megadva a tomb eleresnel! " + id->getName());
            context.addError();
            return nullptr;
        }

        std::vector<llvm::Value*> indexValues;
        for (auto i : *indexExprs)
        {
            auto expr = i->codeGen(context);
            if (!expr)
            {
                Node::printError(loc, "Index nem kiertekheto!");
                context.addError();
                return nullptr;
            }

            if (expr->getType() != context.getGenericIntegerType())
            {
                Node::printError(loc, "Index nem egesz tipus: " + id->getName());
                context.addError();
                return nullptr;
            }

            expr = builder.CreateSub(expr, builder.getInt32(1));
            indexValues.push_back(expr);
        }

        for (auto it = indexValues.begin(); it != indexValues.end(); ++it)
        {
            std::vector<llvm::Value*> ptr_indices;
            llvm::ConstantInt* const_int32_0 = llvm::ConstantInt::get(context.getModule()->getContext(), llvm::APInt(32, 0));
            ptr_indices.push_back(*it);

            if (!var->getAllocatedType())
            {
                Node::printError(loc, "Ez az index nem mutat tombre!");
                context.addError();
                return nullptr;
            }

            auto loadedVector = new llvm::LoadInst(var->getAllocatedType(), var, "load_var", context.currentBlock());
            llvm::Value* ptr = builder.CreateInBoundsGEP(var_struct_type, loadedVector, ptr_indices, "get_struct_element");
            auto value = new llvm::LoadInst(var_struct_type, ptr, "load_ptr_struct", context.currentBlock());

            if (std::next(it) == indexValues.end())
            {
                return value;
            }
            else
            {
                auto nextAlloca = llvm::dyn_cast<llvm::AllocaInst>(value);
                var = nextAlloca;
            }
        }

        return nullptr;
    }

    NodeType getType() override
    {
        return NodeType::Expression;
    }

    std::string str() override
    {
        return "[ VectorAccess(id=" + id->getName() + ") ]" ;
    }

    ExpressionList* getIndexExpressions()
    {
        return indexExprs;
    }
};

class Return : public Statement 
{
public:
    Expression* expr;

    Return(location loc) : expr(nullptr)
    {
        this->loc = loc;
    }

    Return(Expression* expr, location loc)
        : expr(expr)
    {
        this->loc = loc;
    }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        auto Builder = context.getBuilder();
        Builder->SetInsertPoint(context.currentBlock());

        context.getDbgInfo()->emitLocation(this);

        if (!expr)
        {
            return Builder->CreateRet(0);
            //return llvm::ReturnInst::Create(context.getGlobalContext(), 0, context.currentBlock());
        }

        llvm::Value* retValue = expr->codeGen(context);

        if (!retValue)
        {
            return nullptr;
        }

        return Builder->CreateRet(retValue);
        //return llvm::ReturnInst::Create(context.getGlobalContext(), retValue, context.currentBlock());
    }

    std::string str() override
    {
        return "[ Return ]";
    }

    NodeType getType() override
    {
        return NodeType::Expression;
    }

    bool hasRetExpression() const
    {
        return expr;
    }

    Expression* getRetExpression()
    {
        return expr;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitReturn(this);
    }
};

class ExpressionStatement : public Statement
{
public:
    Expression* expression;

    ExpressionStatement(Expression* expression, location loc)
        : expression(expression)
    {
        this->loc = loc;
    }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        context.getDbgInfo()->emitLocation(this);
        return expression->codeGen(context);
    }

    NodeType getType() override
    {
        return NodeType::Expression;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitExpressionStatement(this);
    }

    std::string str() override
    {
        return "[expression statement]";
    }

    Expression* getExpression()
    {
        return expression;
    }
};

class ConditionalExpression : public Expression
{
    Identifier* type;
    Expression* condExpr;
    Expression* thenExpr;
    Expression* elseExpr;

public:
    ConditionalExpression(Expression* condExpr, Expression* thenExpr,
                         location loc) : condExpr(condExpr),
        thenExpr(thenExpr), elseExpr(nullptr)
    {
        this->loc = loc;
    }

    ConditionalExpression(Expression* condExpr, Expression* thenExpr,
                         Expression* elseExpr, location loc) : condExpr(condExpr),
        thenExpr(thenExpr), elseExpr(elseExpr)
    {
        this->loc = loc;
    }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        context.getDbgInfo()->emitLocation(this);
        llvm::Value* comp = condExpr->codeGen(context);

        if (comp == nullptr)
        {
            Node::printError(loc, "A logikai kifejezes nem kiertekelheto.");
            context.addError();
            return nullptr;
        }

        if (!comp->getType()->isIntegerTy(1))
        {
            Node::printError(loc, "Az elagazas feltetele nem logikai kifejezes!");
            context.addError();
            return nullptr;
        }

        llvm::Function* function = context.currentBlock()->getParent();
        llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(context.getGlobalContext(), "then", function);
        llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context.getGlobalContext(), "else");

        llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(context.getGlobalContext(), "merge");

        llvm::BranchInst::Create(thenBlock, elseBlock, comp, context.currentBlock());

        bool needMergeBlock = false;

        context.newScope(thenBlock, context.currentDebugScope(), ScopeType::CodeBlock);

        llvm::Value* thenValue = thenExpr->codeGen(context);
        if (thenValue == nullptr)
        {
            Node::printError(loc, "Az elagazas igaz aga nem kiertekelheto!");
            context.addError();
            return nullptr;
        }

        if (context.currentBlock()->getTerminator() == nullptr)
        {
            llvm::BranchInst::Create(mergeBlock, context.currentBlock());
            needMergeBlock = true;
        }

        function->getBasicBlockList().push_back(elseBlock);
        context.endScope();

        context.newScope(elseBlock);
        llvm::Value* elseValue = nullptr;
        if (elseExpr != nullptr)
        {
            elseValue = elseExpr->codeGen(context);
        }

        if (context.currentBlock()->getTerminator() == nullptr)
        {
            llvm::BranchInst::Create(mergeBlock, context.currentBlock());
            needMergeBlock = true;
        }
        context.endScope();

        if (needMergeBlock)
        {
            function->getBasicBlockList().push_back(mergeBlock);
            context.setInsertPoint(mergeBlock);
        }

        //llvm::verifyFunction(*function, &llvm::outs()
        //{
        //    Node::printError(loc, "Fuggveny generalasi hiba tortent!");
        //    context.addError();
        //    return nullptr;
        //}

        //context.currentBlock()->getParent()->viewCFG();

        //function->viewCFG();

        auto PN = llvm::PHINode::Create(thenValue->getType(), 2, "expriftmp", context.currentBlock());

        PN->addIncoming(thenValue, thenBlock);
        PN->addIncoming(elseValue, elseBlock);
        return PN;
    }

    NodeType getType() override
    {
        return NodeType::Expression;
    }

    std::string str() override
    {
        return "[ ConditionalExpression ]";
    }
};

class Conditional : public Statement
{
public:
    Expression* condExpr;
    Expression* thenExpr;
    Expression* elseExpr;

    Conditional(Expression* condExpr, Expression* thenExpr,
                         location loc) : condExpr(condExpr),
        thenExpr(thenExpr), elseExpr(nullptr)
    {
        this->loc = loc;
    }

    Conditional(Expression* condExpr, Expression* thenExpr,
                         Expression* elseExpr, location loc) : condExpr(condExpr),
        thenExpr(thenExpr), elseExpr(elseExpr)
    {
        this->loc = loc;
    }

    void accept(Visitor& visitor)
    {
        visitor.visitConditional(this);
    }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        context.getDbgInfo()->emitLocation(this);
        llvm::Value* comp = condExpr->codeGen(context);

        if (comp == nullptr)
        {
            Node::printError(loc, "A logikai kifejezes nem kiertekelheto.");
            context.addError();
            return nullptr;
        }

        if (!comp->getType()->isIntegerTy(1))
        {
            Node::printError(loc, "Az elagazas feltetele nem logikai kifejezes!");
            context.addError();
            return nullptr;
        }

        llvm::Function* function = context.currentBlock()->getParent();
        llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(context.getGlobalContext(), "then", function);
        llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context.getGlobalContext(), "else");

        llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(context.getGlobalContext(), "merge");

        llvm::BranchInst::Create(thenBlock, elseBlock, comp, context.currentBlock());

        bool needMergeBlock = false;

        context.newScope(thenBlock, context.currentDebugScope(), ScopeType::CodeBlock);

        llvm::Value* thenValue = thenExpr->codeGen(context);
        if (thenValue == nullptr)
        {
            Node::printError(loc, "Az elagazas igaz aga nem kiertekelheto!");
            context.addError();
            return nullptr;
        }

        if (context.currentBlock()->getTerminator() == nullptr)
        {
            llvm::BranchInst::Create(mergeBlock, context.currentBlock());
            needMergeBlock = true;
        }

        function->getBasicBlockList().push_back(elseBlock);
        context.endScope();

        context.newScope(elseBlock);
        llvm::Value* elseValue = nullptr;
        if (elseExpr != nullptr)
        {
            elseValue = elseExpr->codeGen(context);
        }

        if (context.currentBlock()->getTerminator() == nullptr)
        {
            llvm::BranchInst::Create(mergeBlock, context.currentBlock());
            needMergeBlock = true;
        }
        context.endScope();

        if (needMergeBlock)
        {
            function->getBasicBlockList().push_back(mergeBlock);
            context.setInsertPoint(mergeBlock);
        }

        //llvm::verifyFunction(*function, &llvm::outs()
        //{
        //    Node::printError(loc, "Fuggveny generalasi hiba tortent!");
        //    context.addError();
        //    return nullptr;
        //}

        //context.currentBlock()->getParent()->viewCFG();

        //function->viewCFG();
        return mergeBlock;
    }
    
    NodeType getType() override
    {
        return NodeType::Statement;
    }

    std::string str() override
    {
        return "[ ConditionalStatement ]";
    }

    Expression* getCondExpression()
    {
        return condExpr;
    }

    Expression* getThenExpression()
    {
        return thenExpr;
    }

    Expression* getElseExpression()
    {
        return elseExpr;
    }

};

class ModifyVector : public Statement
{
public:
    Identifier* id;
    ExpressionList* indexExprs;
    Expression* newval;

    ModifyVector(VectorAccess* access, Expression* newval, location loc)
        : id(access->id), indexExprs(access->indexExprs), newval(newval)
    {
        this->loc = loc;
    }

    ModifyVector(Identifier* id, ExpressionList* indexExprs, Expression* newval, bool checkMaxSize = true)
        : id(id), indexExprs(indexExprs), newval(newval)
    {
        this->loc = loc;
    }

    llvm::Value* codeGen(CodeGenContext& context)
    {
        context.getDbgInfo()->emitLocation(this);

        llvm::IRBuilder<> builder(context.getGlobalContext());
        builder.SetInsertPoint(context.currentBlock());

        llvm::AllocaInst* var = context.findVariable(id->getName());

        if (!var)
        {
            Node::printError(loc, "Nincs deklaralva ez a valtozo: " + id->getName());
            context.addError();
            return nullptr;
        }

        if (var->getAllocatedType()->getNumContainedTypes() == 0)
        {
            Node::printError(loc, "Nem tomb tipus: " + id->getName());
            context.addError();
            return nullptr;
        }

        auto var_type = var->getAllocatedType();
        auto var_struct_type = var->getAllocatedType()->getContainedType(0);

        if (!var_struct_type)
        {
            Node::printError(loc, "Variable " + id->getName() + " nem vector,, hanem " + context.getType(id->getName()));
            context.addError();
            return nullptr;
        }

        if (var_type->getTypeID() != llvm::StructType::PointerTyID)
        {
            Node::printError(loc, "Variable " + id->getName() + " tipusa nem egyezik a vektorokkal, hanem " + context.getType(id->getName()));
            context.addError();
            return nullptr;
        }

        if (indexExprs->empty())
        {
            Node::printError(loc, "Hianyzo index kifejezes");
            context.addError();
            return nullptr;
        }

        std::vector<llvm::Value*> indexValues;
        for (auto i : *indexExprs)
        {
            auto expr = i->codeGen(context);
            if (!expr)
            {
                Node::printError(loc, "Index kifejezes nem legeneralhato!");
                context.addError();
                return nullptr;
            }

            indexValues.push_back(expr);
        }

        auto firstIndex = indexValues[0];

        if (firstIndex->getType() != context.getGenericIntegerType())
        {
            Node::printError(loc, "Index kifejezes nem egesz");
            context.addError();
            return nullptr;
        }

        // minus one, pseudo indexing goes from 1!
        firstIndex = builder.CreateSub(firstIndex, builder.getInt32(1));

        llvm::Value* newVal = newval->codeGen(context);

        if (!newVal)
        {
            Node::printError(loc, "Az uj ertek nem legeneralhato");
            context.addError();
            return nullptr;
        }

        if (newVal->getType() != var_struct_type)
        {
            Node::printError(loc, "Az uj ertek tipusa nem egyezik a tombben tarolhato tipussal!");
            context.addError();
            return nullptr;
        }

        for (auto it = indexValues.begin(); it != indexValues.end(); ++it)
        {
            std::vector<llvm::Value*> ptr_indices;
            ptr_indices.push_back(firstIndex);

            if (!var->getAllocatedType())
            {
                Node::printError(loc, "Ez az index nem mutat tombre!");
                context.addError();
                return nullptr;
            }

            auto loadedVector = context.getBuilder()->CreateLoad(var->getAllocatedType(), var, "load_var");
            llvm::Value* ptr = builder.CreateInBoundsGEP(var_struct_type, loadedVector, ptr_indices, "get_struct_element");
            auto value = builder.CreateStore(newVal, ptr, "store_ptr_struct");

            //auto loadedVector = new llvm::LoadInst(var->getAllocatedType(), var, "load_var", context.currentBlock());
            //llvm::Value* ptr = builder.CreateInBoundsGEP(var_struct_type, loadedVector, ptr_indices, "get_struct_element");
            //auto value = new llvm::StoreInst(newVal, ptr, "store_ptr_struct", context.currentBlock());

            if (std::next(it) == indexValues.end())
            {
                return value;
            }
            else
            {
                auto nextAlloca = llvm::dyn_cast<llvm::AllocaInst>(value);
                var = nextAlloca;
            }
        }

        return nullptr;
    }

    NodeType getType() override
    {
        return NodeType::Expression;
    }

    std::string str() override
    {
        return "[ ModifyVector(id=" + id->getName() + ")]";
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitModifyVector(this);
    }

    ExpressionList* getIndexExpressions()
    {
        return indexExprs;
    }

    Expression* getNewValue()
    {
        return newval;
    }
};


class DoWhileLoop : public Statement
{
public:
    Expression* condition;
    Block* loopBlock;

    DoWhileLoop(Expression* condition, Block* loopBlock, location loc) 
                : condition(condition), loopBlock(loopBlock) { this->loc = loc; }

    llvm::Value* codeGen(CodeGenContext& context) override
    {
        context.getDbgInfo()->emitLocation(this);
        llvm::Function* function = context.currentBlock()->getParent();
        llvm::BasicBlock* loopBB = llvm::BasicBlock::Create(context.getGlobalContext(), "loop", function);
        llvm::BasicBlock* condBB = llvm::BasicBlock::Create(context.getGlobalContext(), "condition");
        llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context.getGlobalContext(), "merge");

        //function->getBasicBlockList().push_back(loopBB);
        llvm::BranchInst::Create(loopBB, context.currentBlock());

        context.newScope(loopBB);
        llvm::Value* loopValue = loopBlock->codeGen(context);

        if (!loopValue)
        {
            Node::printError(loc, "A ciklus blokkaja nem kiertekelheto.");
            context.addError();
            return nullptr;
        }

        llvm::BranchInst::Create(condBB, context.currentBlock());
        function->getBasicBlockList().push_back(condBB);
        context.endScope();

        context.newScope(condBB);
        llvm::Value* condValue = condition->codeGen(context);
        if (!condValue)
        {
            Node::printError(loc, "A ciklus logikai kifejezese nem kiertekelheto.");
            context.addError();
            return nullptr;
        }

        if (!condValue->getType()->isIntegerTy(1))
        {
            Node::printError(loc, "A ciklus feltetele nem logikai kifejezes.");
            context.addError();
            return nullptr;
        }

        llvm::BranchInst::Create(loopBB, mergeBB, condValue, context.currentBlock());

        function->getBasicBlockList().push_back(mergeBB);
        context.endScope();
        context.setInsertPoint(mergeBB);
        return mergeBB;
    }

    NodeType getType() override
    {
        return NodeType::Expression;
    }

    std::string str() override
    {
        return "[ DoWhileLoop ]";
    }

    Expression* getCondition()
    {
        return condition;
    }

    Block* getLoopBlock()
    {
        return loopBlock;
    }

    void accept(Visitor& visitor) override
    {
        visitor.visitDoWhileLoop(this);
    }
};
}
#endif
