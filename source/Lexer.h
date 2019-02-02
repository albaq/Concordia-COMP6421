//
// Created by Alvaro Tolosa on 2019-01-29.
//

#ifndef SIMPLECOMPILER_LEXER_H
#define SIMPLECOMPILER_LEXER_H


#include "CharacterScanner.h"
#include "Token.h"

class Lexer {
    CharacterScanner* scanner;
    bool space(char c) const {
        return c == '\n' || c == '\t' || c == ' ';
    }
public:
    Lexer(CharacterScanner* scanner) : scanner(scanner) { }
    Token nextToken();
};

#endif //SIMPLECOMPILER_LEXER_H
