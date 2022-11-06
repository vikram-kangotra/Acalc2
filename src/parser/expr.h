#pragma once

#include <memory>
#include "../token/token.h"

namespace Acalc {

    class ExprVisitor {
        public:
            virtual float visitLiteralExpr(class LiteralExpr* expr) = 0;
            virtual float visitUnaryExpr(class UnaryExpr* expr) = 0;
            virtual float visitBinaryExpr(class BinaryExpr* expr) = 0;
            virtual float visitIntegExpr(class IntegExpr* expr) = 0;
            virtual float visitIdentifierExpr(class IdentifierExpr* expr) = 0;
    };

    class Expr {
        public:
            virtual float accept(ExprVisitor* visitor) = 0;
    };

    class LiteralExpr : public Expr {
        public:
            LiteralExpr(float val) : val{val} {}

            float accept(ExprVisitor* visitor) override {
                return visitor->visitLiteralExpr(this);
            }

            float getValue() const { return val; }
        private:
            float val;
    };

    class UnaryExpr : public Expr {
        public:
            UnaryExpr(std::unique_ptr<Expr>&& expr, Token op)
            : op{op}, expr{std::move(expr)} {}

            float accept(ExprVisitor* visitor) override {
                return visitor->visitUnaryExpr(this);
            }

            Token getOperator() const { return op; }
            Expr* getExpr() const { return expr.get(); }
        private:
            Token op;
            std::unique_ptr<Expr> expr;
    };

    class BinaryExpr : public Expr {
        public:
            BinaryExpr(std::unique_ptr<Expr>&& left, Token op, std::unique_ptr<Expr>&& right)
            : left{std::move(left)}, op{op}, right{std::move(right)} {}

            float accept(ExprVisitor* visitor) override {
                return visitor->visitBinaryExpr(this);
            }

            Expr* getLeft() const { return left.get(); }
            Token getOperator() const { return op; }
            Expr* getRight() const { return right.get(); }
        private:
            std::unique_ptr<Expr> left;
            Token op;
            std::unique_ptr<Expr> right;
    };

    class IntegExpr : public Expr {
        public:
            IntegExpr(std::unique_ptr<Expr>&& formula, std::unique_ptr<Expr>&& limit_start, std::unique_ptr<Expr>&& limit_end)
            : formula{std::move(formula)}, limit_start{std::move(limit_start)}, limit_end{std::move(limit_end)} {}

            float accept(ExprVisitor* visitor) override {
                return visitor->visitIntegExpr(this);
            }

            Expr* getFormula() const { return formula.get(); }
            Expr* getLimitStart() const { return limit_start.get(); }
            Expr* getLimitEnd() const { return limit_end.get(); }

        private:
            std::unique_ptr<Expr> formula;
            std::unique_ptr<Expr> limit_start;
            std::unique_ptr<Expr> limit_end;
    };

    class IdentifierExpr : public Expr {
        public:
            IdentifierExpr(const std::string& variable)
            : variable{variable} {}

            float accept(ExprVisitor* visitor) override {
                return visitor->visitIdentifierExpr(this);
            }

            const std::string& getVariable() const { return variable; }
        private:
            const std::string variable;
    };

    class StmtVisitor {
        public:
            virtual void visitExprStmt(class ExprStmt*) = 0;
            virtual void visitPrintStmt(class PrintStmt*) = 0;
            virtual void visitVariableStmt(class VariableStmt*) = 0;
            virtual void visitGraphStmt(class GraphStmt*) = 0;
            virtual void visitPlotStmt(class PlotStmt*) = 0;
            virtual void visitLineStmt(class LineStmt*) = 0;
            virtual void visitShowStmt(class ShowStmt*) = 0;
            virtual void visitHideStmt(class HideStmt*) = 0;
            virtual void visitClearStmt(class ClearStmt*) = 0;
            virtual void visitCleanStmt(class CleanStmt*) = 0;
            virtual void visitExitStmt(class ExitStmt*) = 0;
    };

    class Stmt {
        public:
            virtual void accept(StmtVisitor* visitor) = 0;
    };

    class ExprStmt : public Stmt {
        public:
            ExprStmt(std::unique_ptr<Expr>&& expr) : expr{std::move(expr)} {}

            void accept(StmtVisitor* visitor) override {
                visitor->visitExprStmt(this);
            }
            Expr* getExpr() { return expr.get(); }
        private:
            std::unique_ptr<Expr> expr;
    };

    class PrintStmt : public Stmt {
        public:
            PrintStmt(std::unique_ptr<Expr>&& expr) : expr{std::move(expr)} {}

            void accept(StmtVisitor* visitor) override {
                visitor->visitPrintStmt(this);
            }

            Expr* getExpr() { return expr.get(); }
        private:
            std::unique_ptr<Expr> expr;
    };

    class VariableStmt : public Stmt {
        public:
            VariableStmt(const std::string& variable, std::unique_ptr<Expr>&& expr)
            : variable{variable}, expr{std::move(expr)} {}

            void accept(StmtVisitor* visitor) override {
                visitor->visitVariableStmt(this);
            }

            const std::string& getVariable() const { return variable; }
            Expr* getExpr() { return expr.get(); }
        private:
            const std::string variable;
            std::unique_ptr<Expr> expr;
    };

    class GraphStmt : public Stmt {
        public:
            GraphStmt(std::unique_ptr<Expr> expr) : expr{std::move(expr)} {}

            void accept(StmtVisitor* visitor) override {
                visitor->visitGraphStmt(this);
            }

            Expr* getExpr() { return expr.get(); }
        private:
            std::unique_ptr<Expr> expr;
    };

    class PlotStmt : public Stmt {
        public:
            PlotStmt(std::unique_ptr<Expr>&& a, std::unique_ptr<Expr>&& b)
            : first{std::move(a)}, second{std::move(b)} {}

            void accept(StmtVisitor* visitor) override {
                visitor->visitPlotStmt(this);
            }

            Expr* getFirstExpr() { return first.get(); }
            Expr* getSecondExpr() { return second.get(); }
        private:
            std::unique_ptr<Expr> first;
            std::unique_ptr<Expr> second;
    };

    class LineStmt : public Stmt {
        public:
            LineStmt(std::unique_ptr<Expr>&& a, std::unique_ptr<Expr>&& b, std::unique_ptr<Expr>&& c, std::unique_ptr<Expr>&& d)
            : first{std::move(a)}, second{std::move(b)}, third{std::move(c)}, fourth{std::move(d)} {}

            void accept(StmtVisitor* visitor) override {
                visitor->visitLineStmt(this);
            }

            Expr* getFirstExpr() { return first.get(); }
            Expr* getSecondExpr() { return second.get(); }
            Expr* getThirdExpr() { return third.get(); }
            Expr* getFourthExpr() { return fourth.get(); }
        private:
            std::unique_ptr<Expr> first;
            std::unique_ptr<Expr> second;
            std::unique_ptr<Expr> third;
            std::unique_ptr<Expr> fourth;
    };

    class ShowStmt : public Stmt {
        public:
            void accept(StmtVisitor* visitor) override {
                visitor->visitShowStmt(this);
            }
    };

    class HideStmt : public Stmt {
        public:
            void accept(StmtVisitor* visitor) override {
                visitor->visitHideStmt(this);
            }
    };

    class ClearStmt : public Stmt {
        public:
            void accept(StmtVisitor* visitor) override {
                visitor->visitClearStmt(this);
            }
    };

    class CleanStmt : public Stmt {
        public:
            void accept(StmtVisitor* visitor) override {
                visitor->visitCleanStmt(this);
            }
    };


    class ExitStmt : public Stmt {
        public:
            void accept(StmtVisitor* visitor) override {
                visitor->visitExitStmt(this);
            }
    };
}
