//
// Created by Alvaro Tolosa on 2019-01-29.
//

#include "Token.h"

static std::map<std::string,TokenType> keywords = {
        {"if", TokenType::KEYWORD_IF},
        {"then", TokenType::KEYWORD_THEN},
        {"else", TokenType::KEYWORD_ELSE},
        {"for", TokenType::KEYWORD_FOR},
        {"class", TokenType::KEYWORD_CLASS},
        {"integer", TokenType::KEYWORD_INTEGER},
        {"float", TokenType::KEYWORD_FLOAT},
        {"read", TokenType::KEYWORD_READ},
        {"write", TokenType::KEYWORD_WRITE},
        {"return", TokenType::KEYWORD_RETURN},
        {"main", TokenType::KEYWORD_MAIN}
};

std::string tokenTypeToString(const TokenType& tokenType) {
    std::string answer;
    switch (tokenType) {
        case TokenType::EQUALS: answer = "EQUALS"; break;
        case TokenType::NOT_EQUALS: answer = "NOT_EQUALS"; break;
        case TokenType::LESS_THAN: answer = "LESS_THAN"; break;
        case TokenType::GREATER_THAN: answer = "GREATER_THAN"; break;
        case TokenType::LESS_OR_EQUAL_THAN: answer = "LESS_OR_EQUAL_THAN"; break;
        case TokenType::GREATER_OR_EQUAL_THAN: answer = "GREATER_OR_EQUAL_THAN"; break;
        case TokenType::SEMICOLON: answer = "SEMICOLON"; break;
        case TokenType::COMMA: answer = "COMMA"; break;
        case TokenType::DOT: answer = "DOT"; break;
        case TokenType::COLON: answer = "COLON"; break;
        case TokenType::DOUBLE_COLON: answer = "DOUBLE_COLON"; break;
        case TokenType::PLUS: answer = "PLUS"; break;
        case TokenType::MINUS: answer = "MINUS"; break;
        case TokenType::ASTERISK: answer = "ASTERISK"; break;
        case TokenType::SLASH: answer = "SLASH"; break;
        case TokenType::ASSIGN: answer = "ASSIGN"; break;
        case TokenType::AND: answer = "AND"; break;
        case TokenType::BANG: answer = "BANG"; break;
        case TokenType::OR: answer = "OR"; break;
        case TokenType::LEFT_PAREN: answer = "LEFT_PAREN"; break;
        case TokenType::RIGHT_PAREN: answer = "RIGHT_PAREN"; break;
        case TokenType::LEFT_BRACE: answer = "LEFT_BRACE"; break;
        case TokenType::RIGHT_BRACE: answer = "RIGHT_BRACE"; break;
        case TokenType::LEFT_BRACKET: answer = "LEFT_BRACKET"; break;
        case TokenType::RIGTH_BRACKET: answer = "RIGHT_BRACKET"; break;
        case TokenType::BLOCK_COMMENT: answer = "BLOCK_COMMENT"; break;
        case TokenType::LINE_COMMENT: answer = "LINE_COMMENT"; break;
        case TokenType::KEYWORD_IF: answer = "IF"; break;
        case TokenType::KEYWORD_THEN: answer = "THEN"; break;
        case TokenType::KEYWORD_ELSE: answer = "ELSE"; break;
        case TokenType::KEYWORD_FOR: answer = "FOR"; break;
        case TokenType::KEYWORD_CLASS: answer = "CLASS"; break;
        case TokenType::KEYWORD_INTEGER: answer = "INTEGER"; break;
        case TokenType::KEYWORD_FLOAT: answer = "FLOAT"; break;
        case TokenType::KEYWORD_READ: answer = "READ"; break;
        case TokenType::KEYWORD_WRITE: answer = "WRITE"; break;
        case TokenType::KEYWORD_RETURN: answer = "RETURN"; break;
        case TokenType::KEYWORD_MAIN: answer = "MAIN"; break;
        case TokenType::NUMBER_INTEGER: answer = "NUM_INTEGER"; break;
        case TokenType::NUMBER_FLOAT: answer = "NUM_FLOAT"; break;
        case TokenType::INVALID: answer = "INVALID"; break;
        case TokenType::IDENT: answer = "ID"; break;
        case TokenType::END_OF_FILE: answer = "END_OF_FILE"; break;
        default:
            answer = "UNRECOGNIZED";
            break;
    }
    return answer;
}

TokenType tokenTypeByKeyword(const std::string& value) {
    auto response = keywords.find(value);
    if(response != keywords.end()) return response->second;
    else return TokenType::INVALID;
}

bool isKeyword(const std::string& value) {
    return keywords.find(value) != keywords.end();
}

