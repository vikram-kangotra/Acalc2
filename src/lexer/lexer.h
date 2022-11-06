#pragma once

#include <string>
#include <vector>
#include "../token/token.h"

namespace Acalc {

    class Lexer {
        public:
            Lexer(const std::string& code) : code{code}, cur{} {}

            std::vector<Token> getTokens();
            Token getNextToken();

        private:
            bool isAtEnd() const;
            char peek() const { return code[cur]; }
            void advance() { ++cur; }

            void skipWhitespace();

            Token number();
            Token identifier();

        private:
            const std::string code;
            size_t cur;
    };
}
