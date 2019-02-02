//
// Created by Alvaro Tolosa on 2019-01-29.
//

#ifndef SIMPLECOMPILER_TOKEN_H
#define SIMPLECOMPILER_TOKEN_H

#include <string>
#include <map>
#include <ostream>

enum class TokenType {
    EQUALS, // ==
    NOT_EQUALS, // <>
    LESS_THAN, // <
    GREATER_THAN, // >
    LESS_OR_EQUAL_THAN, // <=
    GREATER_OR_EQUAL_THAN, // >=
    SEMICOLON,// ;
    COMMA, // ,
    DOT, // .
    COLON, // :
    DOUBLE_COLON, // ::
    PLUS, // +
    MINUS, // -
    ASTERISK, // *
    SLASH, // /
    ASSIGN, // =
    AND, // &&
    BANG, // !
    OR, // ||
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE, // {
    RIGHT_BRACE, // }
    LEFT_BRACKET, // [
    RIGTH_BRACKET, // ]
    BLOCK_COMMENT,
    LINE_COMMENT, // //

    // keywords
    KEYWORD_IF,
    KEYWORD_THEN,
    KEYWORD_ELSE,
    KEYWORD_FOR,
    KEYWORD_CLASS,
    KEYWORD_INTEGER,
    KEYWORD_FLOAT,
    KEYWORD_READ,
    KEYWORD_WRITE,
    KEYWORD_RETURN,
    KEYWORD_MAIN,

    INVALID,
    IDENT,
    END_OF_FILE,
    NUMBER_FLOAT,
    NUMBER_INTEGER

};

std::string tokenTypeToString(const TokenType& tokenType);
bool isKeyword(const std::string& value);
TokenType tokenTypeByKeyword(const std::string& value);

class Token {
    std::string tokenValue;
    TokenType tokenType;
    int lineNumber;
    int columnNumber;
public:
    Token(TokenType type, const std::string& value) : tokenType(type), tokenValue(value), lineNumber(-1), columnNumber(-1) { }

    Token(TokenType type, const std::string& value, int lineNumber, int columnNumber) : tokenType(type),
    tokenValue(value),
    lineNumber(lineNumber),
    columnNumber(columnNumber) { }

    std::string value() const {
        return tokenValue;
    }

    TokenType type() const {
        return tokenType;
    }

    std::pair<int, int> location() {
        return std::pair<int, int>(lineNumber, columnNumber);
    }

    friend std::ostream& operator<< (std::ostream& stream, Token& token) {
        stream<<token.location().first<<"-"<<token.location().second<<": "<< token.value()<<" "<<tokenTypeToString(token.type());
        return stream;
    }

    std::string toString() {
        return tokenTypeToString(tokenType);
    }
};

#endif //SIMPLECOMPILER_TOKEN_H