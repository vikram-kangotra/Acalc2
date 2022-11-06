#pragma once

#include <memory>
#include <cmath>
#include <iostream>
#include "../parser/expr.h"
#include "../environment/environment.h"
#include "../graph/engine.h"

namespace Acalc {

    class Interpreter : public ExprVisitor, public StmtVisitor {
        public:
            void interpret(const std::unique_ptr<Stmt>& expr) {
                expr->accept(this);
            }

        private:

            void visitExprStmt(ExprStmt* es) override {
                es->getExpr()->accept(this);
            }

            void visitPrintStmt(PrintStmt* ps) override {
                float val = ps->getExpr()->accept(this);
                std::cout << val << std::endl;
            }

            void visitVariableStmt(VariableStmt* vs) override {
                float val = 0;
                if (vs->getExpr() != nullptr) {
                    val = vs->getExpr()->accept(this);
                }
                env.define(vs->getVariable(), val);
            }

            void visitGraphStmt(GraphStmt* gs) override {
                float start = -400;
                float end = 400;

                for (float i = start; i < end; i+=0.05) {
                    env.define("x", i);
                    float val = gs->getExpr()->accept(this);
                    Graph::Engine::plot(400 + i, 300 - val);
                }
            }

            void visitPlotStmt(PlotStmt* ps) override {
                Graph::Engine::plot(ps->getFirstExpr()->accept(this), ps->getSecondExpr()->accept(this));
            }

            void visitLineStmt(LineStmt* ls) override {
                Graph::Engine::line(ls->getFirstExpr()->accept(this), ls->getSecondExpr()->accept(this), ls->getThirdExpr()->accept(this), ls->getFourthExpr()->accept(this));
            }

            void visitShowStmt(ShowStmt* ss) override {
                Graph::Engine::show();
            }

            void visitHideStmt(HideStmt* hs) override {
                Graph::Engine::hide();
            }

            void visitExitStmt(ExitStmt* es) override {
                Graph::Engine::exit();
                exit(0);
            }

            void visitClearStmt(ClearStmt* cs) override {
#ifdef _WIN32
                system("cls");
#elif __linux__ || __APPLE__ 
                system("clear");
#endif
            }

            void visitCleanStmt(CleanStmt* cs) override {
                Graph::Engine::clean();
            }

            float visitLiteralExpr(LiteralExpr* expr) override {
                return expr->getValue();
            }

            float visitUnaryExpr(UnaryExpr* expr) override {
                float val = expr->getExpr()->accept(this);
                switch (expr->getOperator().type) {
                    case TOKEN_MINUS: return -val;
                    case TOKEN_SIN: return std::sin(val);
                    case TOKEN_COS: return std::cos(val);
                    case TOKEN_TAN: return std::tan(val);
                    case TOKEN_COSEC: return 1.0f / std::sin(val);
                    case TOKEN_SEC: return 1.0f / std::cos(val);
                    case TOKEN_COT: return 1.0f / std::tan(val);
                    case TOKEN_LOG: return std::log(val);
                    case TOKEN_ABS: return std::abs(val);
                    default: break;
                }
                return val;
            }

            float visitBinaryExpr(BinaryExpr* expr) override {
                float left = expr->getLeft()->accept(this);
                float right = expr->getRight()->accept(this);
                switch (expr->getOperator().type) {
                    case TOKEN_PLUS: return left + right; break;
                    case TOKEN_MINUS: return left - right; break;
                    case TOKEN_STAR: return left * right; break;
                    case TOKEN_SLASH: {
                        if (right == 0) {
                            throw std::runtime_error("Error: Division by zero");
                        }
                        return left / right; break;
                    }
                    case TOKEN_POWER: {
                        if (left < 0 && right != std::floor(right)) {
                            throw std::runtime_error("Error: Negative base to non-integer power");
                        }
                        return std::pow(left, right); break;
                    }
                    default:
                        return 0;
                }
                return 0;
            }

            float visitIdentifierExpr(IdentifierExpr* expr) override {
                return env.get(expr->getVariable());
            }

            float visitIntegExpr(IntegExpr* expr) override {
                float limit_start = expr->getLimitStart()->accept(this);
                float limit_end = expr->getLimitEnd()->accept(this);

                float result = 0.0f;
                int n = 100000;

                float h = (limit_end - limit_start) / n;

                for (int i = 1; i <=n; ++i) {
                    env.define("x", limit_start + i * h);
                    result += expr->getFormula()->accept(this);
                }
                
                return result * h;
            }
        private:
            Environment env;
    };
}
