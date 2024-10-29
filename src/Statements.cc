#include "Statements.h"

namespace lox {

void ExpressionStmt::accept(StmtVistiorRef visitor) {
    visitor->visitExpressionStmt(shared_from_this());
}

void PrintStmt::accept(StmtVistiorRef visitor) {
    visitor->visitPrintStmt(shared_from_this());
}

void VarStmt::accept(StmtVistiorRef visitor) {
    visitor->visitVarStmt(shared_from_this());
}

void BlockStmt::accept(StmtVistiorRef visitor) {
    visitor->visitBlockStmt(shared_from_this());
}

} // namespace lox
