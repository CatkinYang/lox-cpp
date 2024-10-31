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

void IfStmt::accept(StmtVistiorRef visitor) {
    visitor->visitIfStmt(shared_from_this());
}

void WhileStmt::accept(StmtVistiorRef visitor) {
    visitor->visitWhileStmt(shared_from_this());
}

void FunStmt::accept(StmtVistiorRef visitor) {
    visitor->visitFunStmt(shared_from_this());
}

void ReturnStmt::accept(StmtVistiorRef visitor) {
    visitor->visitReturnStmt(shared_from_this());
}

void ClassStmt::accept(StmtVistiorRef visitor) {
    visitor->visitClassStmt(shared_from_this());
}

} // namespace lox
