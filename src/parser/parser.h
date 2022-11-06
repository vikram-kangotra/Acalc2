#pragma once

#include <vector>
#include <memory>
#include "../token/token.h"
#include "expr.h"

namespace Acalc {

    class Parser {
        public:
            Parser(const std::vector<Token>& tokens) : tokens{tokens}, cur{} {}

            std::unique_ptr<Stmt> parse();

        private:
            std::unique_ptr<Stmt> statement();
            std::unique_ptr<Stmt> exprStmt();
            std::unique_ptr<Stmt> printStmt();
            std::unique_ptr<Stmt> varDecl();
            std::unique_ptr<Stmt> graphStmt();
            std::unique_ptr<Stmt> plotStmt();
            std::unique_ptr<Stmt> lineStmt();

            std::unique_ptr<Expr> expression();
            std::unique_ptr<Expr> term();
            std::unique_ptr<Expr> factor();
            std::unique_ptr<Expr> unary();
            std::unique_ptr<Expr> primary();

            void consume(TokenType type, const char* msg);
            bool isAtEnd() const { return cur >= tokens.size() || peek().type == TOKEN_EOF; }
            const Token& peek() const { return tokens[cur]; }
            void advance() { ++cur; }
            Token previous() { return tokens[cur-1]; }

            bool match(TokenType type);

            template <typename... T>
            bool match(T&&... type) {
                return (match(std::forward<T>(type)) || ...);
            }

        private:
            std::vector<Token> tokens;
            size_t cur;
    };
}
