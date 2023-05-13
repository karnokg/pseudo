#include "vector.h"
#include "string.h"
#include "parser.h"
#include "compareoperator.h"
#include "binaryoperator.h"
#include "vectoraddelement.h"

namespace Pseudo
{
    Vector::Vector(Identifier* type, ExpressionList* exprs, Expression* maxSizeExpr, location loc)
        : type(type), exprs(exprs), maxSizeExpr(maxSizeExpr) { this->loc = loc; }

    Vector::Vector(Identifier* type, Expression* maxSizeExpr, location loc)
        : type(type), exprs(new ExpressionList()), maxSizeExpr(maxSizeExpr) { this->loc = loc; }

    llvm::Value* Vector::codeGen(CodeGenContext& context)
    {
        llvm::Type* element_type = context.typeOf(type->getName());

        auto builder = context.getBuilder();

        llvm::Value* array_size = maxSizeExpr->codeGen(context);

        if (!array_size)
        {
            Node::printError(loc, "Tomb merete nem generalhato le.");
            context.addError();
            return nullptr;
        }

        context.getDbgInfo()->emitLocation(this);
        return builder->CreateAlloca(element_type, array_size, "vectorresult");

    }

}
