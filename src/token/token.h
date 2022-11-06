#pragma once

#include <string>

namespace Acalc {

    enum TokenType {
        TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
        TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
        TOKEN_EQUAL,
        TOKEN_COMMA, TOKEN_MINUS, TOKEN_PLUS,
        TOKEN_SLASH, TOKEN_STAR,

        TOKEN_VAR, TOKEN_SHOW, TOKEN_HIDE,
        TOKEN_PLOT, TOKEN_CLEAR, TOKEN_EXIT,

        TOKEN_LINE, TOKEN_CLEAN, TOKEN_GRAPH,

        TOKEN_PRINT, TOKEN_SEMICOLON, 
        TOKEN_INTEG, TOKEN_ABS,

        TOKEN_IDENTIFIER, TOKEN_NUMBER,

        TOKEN_POWER, TOKEN_INT,

        TOKEN_SIN, TOKEN_COS, TOKEN_TAN,
        TOKEN_COSEC, TOKEN_SEC, TOKEN_COT,

        TOKEN_LOG,

        TOKEN_ERROR,
        TOKEN_EOF
    };

    struct Token {
        TokenType type;
        std::string lexeme;

        Token(const TokenType type, const std::string& lexeme) : type{type}, lexeme{lexeme} {}
    };

}
