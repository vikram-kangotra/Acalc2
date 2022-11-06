#include "parser.h"
#include "expr.h"
#include <iostream>

// parse        -> statement* EOF
// forStmt      -> "for" "(" (varDecl | exprStmt | ";") expression? ";" expression? ")" statement
// statement    -> exprStmt | printStmt | graphStmt | varDecl | plotStmt | lineStmt |
//                  ("show" | "hide" | "clear" | "clean" | "exit") ";"
// exprStmt     -> expr ";"
// graphStmt    -> "graph" term ";"
// printStmt    -> "print" expr ";"
// varDecl      -> "var" IDENTIFIER ( "=" expression )? ";"
// plotStmt     -> "plot" expr "," expr";"
// lineStmt     -> "line" expr "," expr "," expr "," expr ";"
// expr         -> term | "integ" "(" term "," term "," term ")"
// term         -> factor (("-" | "+") factor)*;
// factor       -> UnaryExpr (("*" | "/") UnaryExpr)*
// unary        -> ("+" | "-" | "sin") unary | primary
// primary      -> num | identifier | "(" expr ")"

namespace Acalc {

    std::unique_ptr<Stmt> Parser::parse() {
        if (isAtEnd()) return nullptr;
        return statement();
    }

    std::unique_ptr<Stmt> Parser::statement() {
        if (match(TOKEN_PRINT)) return printStmt();
        if (match(TOKEN_VAR)) return varDecl();
        if (match(TOKEN_GRAPH)) return graphStmt();
        if (match(TOKEN_PLOT)) return plotStmt();
        if (match(TOKEN_LINE)) return lineStmt();
        if (match(TOKEN_SHOW)) {
            consume(TOKEN_SEMICOLON, "Expected ';' after 'show'.");
            return std::make_unique<ShowStmt>();
        }
        if (match(TOKEN_HIDE)) {
            consume(TOKEN_SEMICOLON, "Expected ';' after 'hide'.");
            return std::make_unique<HideStmt>();
        }
        if (match(TOKEN_CLEAR)) {
            consume(TOKEN_SEMICOLON, "Expected ';' after 'clear'.");
            return std::make_unique<ClearStmt>();
        }
        if (match(TOKEN_CLEAN)) {
            consume(TOKEN_SEMICOLON, "Expected ';' after 'clean'.");
            return std::make_unique<CleanStmt>();
        }
        if (match(TOKEN_EXIT)) {
            consume(TOKEN_SEMICOLON, "Expected ';' after 'exit'.");
            return std::make_unique<ExitStmt>();
        }
        return exprStmt();
    }

    std::unique_ptr<Stmt> Parser::exprStmt() {
        auto expr = expression();
        consume(TOKEN_SEMICOLON, "forgot ';'?");
        return std::make_unique<ExprStmt>(std::move(expr));
    }

    std::unique_ptr<Stmt> Parser::printStmt() {
        auto expr = expression();
        consume(TOKEN_SEMICOLON, "forgot ';'?");
        return std::make_unique<PrintStmt>(std::move(expr));
    }

    std::unique_ptr<Stmt> Parser::varDecl() {
        consume(TOKEN_IDENTIFIER, "identifier expected after 'var'");
        auto iden = previous().lexeme;
        std::unique_ptr<Expr> expr = nullptr;
        if (match(TOKEN_EQUAL)) {
             expr = expression();
        }
        consume(TOKEN_SEMICOLON, "forgot ';'?");
        return std::make_unique<VariableStmt>(iden, std::move(expr));
    }

    std::unique_ptr<Stmt> Parser::graphStmt() {
        auto expr = expression();
        consume(TOKEN_SEMICOLON, "forgot ';'?");
        return std::make_unique<GraphStmt>(std::move(expr));
    }

    std::unique_ptr<Stmt> Parser::plotStmt() {
        auto a = expression();
        consume(TOKEN_COMMA, "Expected ',' after first expression.");
        auto b = expression();
        consume(TOKEN_SEMICOLON, "forgot ';'?");
        return std::make_unique<PlotStmt>(std::move(a), std::move(b));
    }

    std::unique_ptr<Stmt> Parser::lineStmt() {
        auto a = expression();
        consume(TOKEN_COMMA, "Expected ',' after first expression.");
        auto b = expression();
        consume(TOKEN_COMMA, "Expected ',' after second expression.");
        auto c = expression();
        consume(TOKEN_COMMA, "Expected ',' after third expression.");
        auto d = expression();
        consume(TOKEN_SEMICOLON, "forgot ';'?");
        return std::make_unique<LineStmt>(std::move(a), std::move(b), std::move(c), std::move(d));
    }

    std::unique_ptr<Expr> Parser::expression() {
        if (match(TOKEN_INTEG)) {
            consume(TOKEN_LEFT_PAREN, "expected a '('");
            auto formula = term();
            consume(TOKEN_COMMA, "expected a ','");
            auto limit_start = term();
            consume(TOKEN_COMMA, "expected a ','");
            auto limit_end = term();
            consume(TOKEN_RIGHT_PAREN, "expected a ')'");
            return std::make_unique<IntegExpr>(std::move(formula), std::move(limit_start), std::move(limit_end));
        }
        return term();
    }

    std::unique_ptr<Expr> Parser::term() {
        auto left = factor();
        while (match(TOKEN_MINUS, TOKEN_PLUS)) {
            auto op = previous();
            auto right = factor();
            left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
        }
        return left;
    }

    std::unique_ptr<Expr> Parser::factor() {
        auto left = unary();
        while (match(TOKEN_STAR, TOKEN_SLASH, TOKEN_POWER)) {
            auto op = previous();
            auto right = unary();
            left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
        }
        return left;
    }

    std::unique_ptr<Expr> Parser::unary() {
        if (match(TOKEN_MINUS, TOKEN_PLUS, TOKEN_SIN, TOKEN_COS, TOKEN_TAN, TOKEN_COSEC, TOKEN_SEC, TOKEN_COT, TOKEN_LOG, TOKEN_ABS)) {
            auto op = previous();
            auto expr = unary();
            return std::make_unique<UnaryExpr>(std::move(expr), op);
        }
        return primary();
    }

    std::unique_ptr<Expr> Parser::primary() {
        if (match(TOKEN_NUMBER)) {
            float val = std::stof(previous().lexeme);
            return std::make_unique<LiteralExpr>(val);
        }

        if (match(TOKEN_IDENTIFIER)) {
            return std::make_unique<IdentifierExpr>(previous().lexeme);
        }

        if (match(TOKEN_LEFT_PAREN)) {
            auto expr = expression();
            if (match(TOKEN_RIGHT_PAREN)) {
                return expr;
            }
        }
        throw "unrecognised character encountered.";
    }

    bool Parser::match(TokenType type) {
        if (peek().type == type) {
            advance();
            return true;
        }
        return false;
    }

    void Parser::consume(TokenType type, const char* const msg) {
        if (!match(type))
            throw msg;
    }
}
