//
// Created by Alvaro Tolosa on 2019-01-29.
//

#include "Lexer.h"

void backScanner(CharacterScanner* scanner, char c) {
    if(c != 0) scanner->backupChar();
}

bool letter(char c) {
    return (c>='a' && c<='z') || (c>='A' && c<='Z');
}

bool digitNonZero(char c) {
    return c >= '1' && c <= '9';
}

bool digit(char c) {
    return c == '0' || digitNonZero(c);
}

bool alphanum(char c) {
    return c=='_' || letter(c) || digit(c);
}

std::string readId(CharacterScanner* scanner) {
    std::string result;
    char c = scanner->nextChar();
    if (letter(c)) {
        result += c;
        c = scanner->nextChar();
        while(alphanum(c)) {
            result+=c;
            c = scanner->nextChar();
        }
        backScanner(scanner, c);
    }
    return result;
}

std::string readInteger(CharacterScanner* scanner) {
    std::string result;
    char c = scanner->nextChar();

    if(c=='0') {
        result += c;
    } else {
        while(digit(c)) {
            result+=c;
            c = scanner->nextChar();
        }
        backScanner(scanner, c);
    }
    return result;
}

std::string readExponential(CharacterScanner* scanner) {
    std::string result;

    char c = scanner->nextChar();
    if (c == 'e') {
        result += c;
        c = scanner->nextChar();
        bool hasSign = false;
        if (c == '+' || c == '-') {
            result += c;
            c = scanner->nextChar();
            hasSign = true;
        }

        std::string maybeInt;
        if(digit(c)) {
            backScanner(scanner, c);
            maybeInt = readInteger(scanner);
        }

        if(maybeInt.empty()) {
            scanner->backupChar();
            result.pop_back();
            if(hasSign) {
                scanner->backupChar();
                result.pop_back();
            }
        } else {
            result +=maybeInt;
        }
    }
    return result;
}

std::string readPartialNumber(CharacterScanner* scanner) {
    std::string val;

    char c = scanner->nextChar();
    while(digit(c)) {
        val += c;
        c = scanner->nextChar();
    }

    backScanner(scanner, c);

    if(!digitNonZero(val.back())) { //not a valid fraction, it ends in zero, search for the first non zero char
        while(!val.empty() && !digitNonZero(val.back())) {
            val.pop_back();
            scanner->backupChar();
        }
    }
    return val;
}

std::string readFraction(CharacterScanner* scanner) {
    std::string result;
    char c = scanner->nextChar();

    if(c == '.') { //maybe this is a fraction
        result += c;
        c = scanner->nextChar();
        if(c == '0') {
            result += c;
            c = scanner->nextChar();

            if(!digit(c)) {
                backScanner(scanner, c);
            } else if (digit(c)) {
                scanner->backupChar();
                result += readPartialNumber(scanner);


            }
        } else if(digitNonZero(c)) {
            scanner->backupChar();
            result += readPartialNumber(scanner);

        } else { //after the dot there is a no-numeric character, this is definetely not a fraction
            scanner->backupChar();
            scanner->backupChar();
            result.pop_back();
        }
    }
    return result;
}

Token Lexer::nextToken() {
    Token *answer = nullptr;
    //skip all blanks if there are
    char c = scanner->nextChar();
    while(space(c)) {
        c = scanner->nextChar();
    }

    std::pair<int, int> location = scanner->location();

    if (c == 0) answer = new Token(TokenType::END_OF_FILE, "", location.first, location.second);
    else {
        std::string value;
        if(letter(c)) { //it is an identifier or a keyword
            scanner->backupChar();
            value = readId(scanner);
            TokenType tokenType = tokenTypeByKeyword(value);
            answer = new Token(tokenType == TokenType::INVALID ? TokenType::IDENT : tokenType, value, location.first, location.second);
        } else if (digit(c)) { //it is an integer or a float which starts with a 1 to 9 digit
            scanner->backupChar();
            value = readInteger(scanner);
            c = scanner->nextChar();
            if(c == '.') { //possible float, not integer
                scanner->backupChar();
                std::string val = readFraction(scanner);
                if(!val.empty()) { //a number was encounter after the dot (.), se we need to check if we have a 'e'
                    value += val;
                    c = scanner->nextChar();
                    if (c == 'e') { //en 'e' was encounter, so it is possible the number has an exponential port.
                        scanner->backupChar();
                        val = readExponential(scanner);
                        if(!val.empty()) {
                            value += val;
                        }
                    } else {
                        scanner->backupChar();
                    }
                    answer = new Token(TokenType::NUMBER_FLOAT, value, location.first, location.second);
                } else {
                    answer = new Token(TokenType::NUMBER_INTEGER, value, location.first, location.second);
                }
                //if(c)
            } else { //it is an integer for sure
                scanner->backupChar();
                answer = new Token(TokenType::NUMBER_INTEGER, value, location.first, location.second);
            }
        } else { //it is not a letter or a digit, so it is a symbol
            switch (c) {
                case '.':
                    answer = new Token(TokenType::DOT, std::string(1, c), location.first, location.second);
                    break;
                case '+':
                    answer = new Token(TokenType::PLUS, std::string(1, c), location.first, location.second);
                    break;
                case '*':
                    answer = new Token(TokenType::ASTERISK, std::string(1, c), location.first, location.second);
                    break;
                case '=': {
                    c = scanner->nextChar();
                    if (c == '=') {
                        answer = new Token(TokenType::EQUALS, "==", location.first, location.second);
                    } else {
                        backScanner(scanner, c);
                        answer = new Token(TokenType::ASSIGN, "=", location.first, location.second);
                    }
                }
                    break;
                case '<': {
                    c = scanner->nextChar();
                    if (c == '>') {
                        answer = new Token(TokenType::NOT_EQUALS, "<>", location.first, location.second);
                    } else if (c == '=') {
                        answer = new Token(TokenType::LESS_OR_EQUAL_THAN, "<=", location.first, location.second);
                    } else {
                        backScanner(scanner, c);
                        answer = new Token(TokenType::LESS_THAN, "<", location.first, location.second);
                    }
                }
                    break;
                case '>': {
                    c = scanner->nextChar();
                    if (c == '=') {
                        answer = new Token(TokenType::GREATER_OR_EQUAL_THAN, ">=", location.first, location.second);
                    } else {
                        backScanner(scanner, c);
                        answer = new Token(TokenType::GREATER_THAN, ">", location.first, location.second);
                    }
                }
                    break;
                case ';':
                    answer = new Token(TokenType::SEMICOLON, std::string(1, c), location.first, location.second);
                    break;
                case ',':
                    answer = new Token(TokenType::COMMA, std::string(1, c), location.first, location.second);
                    break;
                case '!':
                    answer = new Token(TokenType::BANG, std::string(1, c), location.first, location.second);
                    break;
                case '(':
                    answer = new Token(TokenType::LEFT_PAREN, std::string(1, c), location.first, location.second);
                    break;
                case ')':
                    answer = new Token(TokenType::RIGHT_PAREN, std::string(1, c), location.first, location.second);
                    break;
                case '{':
                    answer = new Token(TokenType::LEFT_BRACE, std::string(1, c), location.first, location.second);
                    break;
                case '}':
                    answer = new Token(TokenType::RIGHT_BRACE, std::string(1, c), location.first, location.second);
                    break;
                case '[':
                    answer = new Token(TokenType::LEFT_BRACKET, std::string(1, c), location.first, location.second);
                    break;
                case ']':
                    answer = new Token(TokenType::RIGTH_BRACKET, std::string(1, c), location.first, location.second);
                    break;
                case ':': {
                    c = scanner->nextChar();
                    if (c == ':') {
                        answer = new Token(TokenType::DOUBLE_COLON, "::", location.first, location.second);
                    } else {
                        backScanner(scanner, c);
                        answer = new Token(TokenType::COLON, ":", location.first, location.second);
                    }
                }
                    break;
                case '&': {
                    c = scanner->nextChar();
                    if (c == '&') {
                        answer = new Token(TokenType::AND, "&&", location.first, location.second);
                    } else {
                        backScanner(scanner, c);
                        answer = new Token(TokenType::INVALID, "&", location.first, location.second);
                    }
                }
                    break;
                case '|': {
                    c = scanner->nextChar();
                    if (c == '|') {
                        answer = new Token(TokenType::OR, "||", location.first, location.second);
                    } else {
                        backScanner(scanner, c);
                        answer = new Token(TokenType::INVALID, "|", location.first, location.second);
                    }
                }
                    break;
                case '/': { //manage '/' and one line comments '//...' and multiline commnets '/*'
                    c = scanner->nextChar();
                    std::string val;
                    if(c == '/') {
                        c = scanner->nextChar();
                        while(c != '\n' && c!= 0) {
                            val+=c;
                            c = scanner->nextChar();
                        }
                        answer = new Token(TokenType::LINE_COMMENT, val, location.first, location.second);
                    } else if (c=='*') { //multiline comment
                        while(true) {
                            c = scanner->nextChar();
                            while(c != '*' && c!= 0) {
                                val+=c;
                                c = scanner->nextChar();
                            }
                            if( c == '*') {
                                c = scanner->nextChar();
                                if(c !='/') {
                                    val.push_back('*');
                                    scanner->backupChar();
                                } else {
                                    answer = new Token(TokenType::BLOCK_COMMENT, val, location.first, location.second);
                                    break;
                                }
                            } else if (c == 0) {
                                answer = new Token(TokenType::INVALID, "No end of multiline comment", location.first, location.second);
                                break;
                            }
                        }
                    }else {
                        scanner->backupChar();
                        answer = new Token(TokenType::SLASH, "/", location.first, location.second);
                    }
                }
                    break;
                case '_': {
                    std::string val = std::string(1, c);
                    c = scanner->nextChar();
                    if(letter(c)) {
                        while (alphanum(c)) {
                            val += c;
                            c = scanner->nextChar();
                        }
                    }
                    answer = new Token(TokenType::INVALID, val, location.first, location.second);
                    scanner->backupChar();
                }
                    break;
                default:
                    answer = new Token(TokenType::INVALID, std::string(1, c), location.first, location.second);
                    break;
            }

        }
    }
    return *answer;
}