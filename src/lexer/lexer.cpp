#include "lexer.h"
#include <iostream>

namespace Acalc {

    std::vector<Token> Lexer::getTokens() {
        std::vector<Token> tokens;

        while (!isAtEnd()) {
            Token token = getNextToken();
            tokens.push_back(token);
            advance();
        }

        tokens.emplace_back(TOKEN_EOF, "");

        return tokens;
    }

    Token Lexer::getNextToken() {

        skipWhitespace();

        switch (peek()) {
            case '(': return Token(TOKEN_LEFT_PAREN, "("); break;
            case ')': return Token(TOKEN_RIGHT_PAREN, ")"); break;
            case '{': return Token(TOKEN_LEFT_BRACE, "{"); break;
            case '}': return Token(TOKEN_RIGHT_BRACE, "}"); break;
            case ',': return Token(TOKEN_COMMA, ","); break;
            case '-': return Token(TOKEN_MINUS, "-"); break;
            case '+': return Token(TOKEN_PLUS, "+"); break;
            case '/': return Token(TOKEN_SLASH, "/"); break;
            case '*': return Token(TOKEN_STAR, "*"); break;
            case '^': return Token(TOKEN_POWER, "^"); break;
            case ';': return Token(TOKEN_SEMICOLON, ";"); break;
            case '=': return Token(TOKEN_EQUAL, "="); break;
            default:
                      if (isdigit(peek())) {
                          return number();
                      } else if (isalpha(peek())) {
                          return identifier();
                      }
        }

        return Token(TOKEN_ERROR, "err");
    }

    void Lexer::skipWhitespace() {
        while (true) {
            switch (peek()) {
                case '\n':
                case '\r':
                case ' ': advance(); break;
                default:
                    return;
            }
        }
    }

    Token Lexer::number() {
        size_t start = cur;

        while (isdigit(peek())) {
            advance();
        }

        if (peek() == '.') {
            advance();
        }

        while (isdigit(peek())) {
            advance();
        }

        --cur;

        return Token(TOKEN_NUMBER, code.substr(start, cur - start + 1));
    }

    Token Lexer::identifier() {
        size_t start = cur;
        advance();

        while (isalpha(peek()) || isdigit(peek())) {
            advance();
        }

        --cur;

        std::string iden = code.substr(start, cur - start + 1);

        switch (iden[0]) {
            case 'a': {
                if (iden == "abs") {
                    return Token(TOKEN_ABS, iden);
                }
            } break;
            case 'c': {
                switch (iden[1]) {
                    case 'o': {
                        switch (iden[2]) {
                            case 's': {
                                if (iden == "cos") {
                                    return Token(TOKEN_COS, iden);
                                } else if (iden == "cosec") {
                                    return Token(TOKEN_COSEC, iden);
                                }
                            } break;
                            case 't': return Token(TOKEN_COT, iden);
                        }
                    } break;
                    case 'l': {
                        if (iden == "clear") {
                            return Token(TOKEN_CLEAR, iden);
                        }
                        if (iden == "clean") {
                            return Token(TOKEN_CLEAN, iden);
                        }
                    } break;
                }
            } break;
            case 'e': {
                if (iden == "exit") {
                    return Token(TOKEN_EXIT, iden);
                }
            } break;
            case 'g': {
                if (iden == "graph") {
                    return Token(TOKEN_GRAPH, iden);
                }
            } break;
            case 'h': {
                if (iden == "hide") {
                    return Token(TOKEN_HIDE, iden);
                }
            }
            case 'i': {
                if (iden == "integ") {
                    return Token(TOKEN_INTEG, iden);
                }
            } break;
            case 'l': {
                switch (iden[1]) {
                    case 'i': {
                        if (iden == "line") {
                            return Token(TOKEN_LINE, iden);
                        }
                    } break;
                    case 'o': {
                        if (iden == "log") {
                            return Token(TOKEN_LOG, iden);
                        }
                    } break;
                }
            }
            case 'p': {
                if (iden == "print") {
                    return Token(TOKEN_PRINT, iden);
                }
                if (iden == "plot") {
                    return Token(TOKEN_PLOT, iden);
                }
            } break;
            case 's': {
                if (iden == "sec") {
                    return Token(TOKEN_SEC, iden);
                }
                if (iden == "sin") {
                    return Token(TOKEN_SIN, iden);
                }
                if (iden == "show") {
                    return Token(TOKEN_SHOW, iden);
                }
            } break;
            case 't': {
                if (iden == "tan") {
                    return Token(TOKEN_TAN, iden);
                }
            } break;
            case 'v': {
                if (iden == "var") {
                    return Token(TOKEN_VAR, iden);
                }
            }
        }
        return Token(TOKEN_IDENTIFIER, iden);
    }

    bool Lexer::isAtEnd() const {
        return cur >= code.length() || peek() == '\0';
    }
}
