#pragma once
#include "Expression.h"
#include "Object.h"
#include "Token.h"
#include <memory>
#include <vector>

namespace lox {

class Stmt;
class ExpressionStmt;
class PrintStmt;
class VarStmt;
class BlockStmt;
class IfStmt;
class WhileStmt;
class ForStmt;
class FunStmt;
class ReturnStmt;

using StmtRef = std::shared_ptr<Stmt>;
using ExpressionStmtRef = std::shared_ptr<ExpressionStmt>;
using PrintStmtRef = std::shared_ptr<PrintStmt>;
using VarStmtRef = std::shared_ptr<VarStmt>;
using BlockStmtRef = std::shared_ptr<BlockStmt>;
using IfStmtRef = std::shared_ptr<IfStmt>;
using WhileStmtRef = std::shared_ptr<WhileStmt>;
using ForStmtRef = std::shared_ptr<ForStmt>;
using FunStmtRef = std::shared_ptr<FunStmt>;
using ReturnStmtRef = std::shared_ptr<ReturnStmt>;

class StmtVisitor {
  public:
    virtual ~StmtVisitor() = default;
    virtual void visitExpressionStmt(ExpressionStmtRef stmt) = 0;
    virtual void visitPrintStmt(PrintStmtRef stmt) = 0;
    virtual void visitVarStmt(VarStmtRef stmt) = 0;
    virtual void visitBlockStmt(BlockStmtRef stmt) = 0;
    virtual void visitIfStmt(IfStmtRef stmt) = 0;
    virtual void visitWhileStmt(WhileStmtRef stmt) = 0;
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

class IfStmt : public Stmt, public std::enable_shared_from_this<IfStmt> {
  public:
    IfStmt(AbstractExpressionRef<Object> condition, StmtRef thenBranch,
           StmtRef elseBranch)
        : m_condition(condition), m_thenBranch(thenBranch),
          m_elseBranch(elseBranch) {};

    virtual void accept(StmtVistiorRef visitor) override;

    auto getCondition() -> AbstractExpressionRef<Object> { return m_condition; }
    auto getThen() { return m_thenBranch; }
    auto getElse() { return m_elseBranch; }

  private:
    AbstractExpressionRef<Object> m_condition;
    StmtRef m_thenBranch;
    StmtRef m_elseBranch;
};

class WhileStmt : public Stmt, public std::enable_shared_from_this<WhileStmt> {
  public:
    WhileStmt(AbstractExpressionRef<Object> condition, StmtRef body)
        : m_condition(condition), m_body(body) {}

    virtual void accept(StmtVistiorRef visitor) override;

    auto getCondition() -> AbstractExpressionRef<Object> { return m_condition; }
    auto getBody() -> StmtRef { return m_body; }

  private:
    AbstractExpressionRef<Object> m_condition;
    StmtRef m_body;
};

class FunStmt : public Stmt, public std::enable_shared_from_this<FunStmt> {
  public:
    auto getName() { return m_name; }
    auto getParams() { return m_params; }
    auto getBody() { return m_body; }

  private:
    TokenRef m_name;
    std::vector<TokenRef> m_params;
    std::vector<StmtRef> m_body;
};

class ReturnStmt : public Stmt,
                   public std::enable_shared_from_this<ReturnStmt> {
  public:
    ReturnStmt(AbstractExpressionRef<Object> value) : m_value(value) {};

    auto getValue() { return m_value; }
    auto getKeyword() { return m_keyword; }

  private:
    TokenRef m_keyword;
    AbstractExpressionRef<Object> m_value;
};

// class Stmt : public Stmt, public std::enable_shared_from_this<Stmt> {
//   public:
//   private:
// };

} // namespace lox
