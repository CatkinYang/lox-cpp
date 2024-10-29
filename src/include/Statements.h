#pragma once
#include "Expression.h"
#include "Object.h"
#include <memory>
#include <vector>

namespace lox {

class Stmt;
class ExpressionStmt;
class PrintStmt;
class VarStmt;
class BlockStmt;
using StmtRef = std::shared_ptr<Stmt>;
using ExpressionStmtRef = std::shared_ptr<ExpressionStmt>;
using PrintStmtRef = std::shared_ptr<PrintStmt>;
using VarStmtRef = std::shared_ptr<VarStmt>;
using BlockStmtRef = std::shared_ptr<BlockStmt>;

class StmtVisitor {
  public:
    virtual ~StmtVisitor() = default;
    virtual void visitExpressionStmt(ExpressionStmtRef stmt) = 0;
    virtual void visitPrintStmt(PrintStmtRef stmt) = 0;
    virtual void visitVarStmt(VarStmtRef stmt) = 0;
    virtual void visitBlockStmt(BlockStmtRef stmt) = 0;
};
using StmtVistiorRef = std::shared_ptr<StmtVisitor>;

class Stmt {
  public:
    virtual void accept(StmtVistiorRef visitor) = 0;
    virtual ~Stmt() = default;
};

class ExpressionStmt : public Stmt,
                       public std::enable_shared_from_this<ExpressionStmt> {
  public:
    ExpressionStmt(AbstractExpressionRef<Object> expr) : m_expr(expr) {}

    virtual void accept(StmtVistiorRef visitor) override;
    auto getExpr() { return m_expr; }

  private:
    AbstractExpressionRef<Object> m_expr;
};

class PrintStmt : public Stmt, public std::enable_shared_from_this<PrintStmt> {
  public:
    PrintStmt(AbstractExpressionRef<Object> expr) : m_expr(expr) {}

    virtual void accept(StmtVistiorRef visitor) override;

    auto getExpr() { return m_expr; }

  private:
    AbstractExpressionRef<Object> m_expr;
};

class VarStmt : public Stmt, public std::enable_shared_from_this<VarStmt> {
  public:
    VarStmt(TokenRef name, AbstractExpressionRef<Object> initializer)
        : m_name(name), m_initializer(initializer) {}

    virtual void accept(StmtVistiorRef visitor) override;

    auto getName() { return m_name; }
    auto getInitExpr() { return m_initializer; }

  private:
    TokenRef m_name;
    AbstractExpressionRef<Object> m_initializer;
};

class BlockStmt : public Stmt, public std::enable_shared_from_this<BlockStmt> {
  public:
    BlockStmt(std::vector<StmtRef> statements) : m_statements(statements) {};

    virtual void accept(StmtVistiorRef visitor) override;

    auto getStmt() -> std::vector<StmtRef> { return m_statements; }

  private:
    std::vector<StmtRef> m_statements;
};

} // namespace lox
