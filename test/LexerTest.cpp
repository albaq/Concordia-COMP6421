//
// Created by Alvaro Tolosa on 2019-01-30.
//

//#define BOOST_TEST_MODULE LexerTest

#include <boost/test/unit_test.hpp>
#include "../source/Lexer.h"
#include "../source/InMemoryScanner.h"

void checkTokenValueAndType(Token token, TokenType expectedType, std::string expectedValue) {
    BOOST_CHECK(expectedType == token.type());
    BOOST_CHECK_EQUAL(expectedValue, token.value());
}

void checkTokenValueAndType(Token token, TokenType expectedType, std::string expectedValue, int expectedRow, int expectedColumn) {
    std::pair<int, int> expected = std::pair<int, int>(expectedRow, expectedColumn);

    BOOST_CHECK(expectedType == token.type());
    BOOST_CHECK_EQUAL(expectedValue, token.value());
    BOOST_CHECK(expected == token.location());
}

BOOST_AUTO_TEST_SUITE(lexer_suite)

    BOOST_AUTO_TEST_CASE(reading_a_zero_then_integer) {
        std::vector<std::string> text;
        text.push_back("0123");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "0", 0, 0);
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "123", 0, 1);
    }

    BOOST_AUTO_TEST_CASE(reading_a_zero_then_float) {
        std::vector<std::string> text;
        text.push_back("01.23");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "0", 0, 0);
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_FLOAT, "1.23", 0, 1);
    }

    BOOST_AUTO_TEST_CASE(reading_a_float_then_zero) {
        std::vector<std::string> text;
        text.push_back("12.340");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_FLOAT, "12.34", 0, 0);
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "0", 0, 5);

    }

    BOOST_AUTO_TEST_CASE(reading_a_zero_then_float_then_zero) {
        std::vector<std::string> text;
        text.push_back("012.340");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "0", 0, 0);
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_FLOAT, "12.34", 0, 1);
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "0", 0, 6);
    }

    BOOST_AUTO_TEST_CASE(reading_a_exp_then_integer) {
        std::vector<std::string> text;
        text.push_back("12.34e01");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_FLOAT, "12.34e0", 0, 0);
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "1", 0, 7);
    }

    BOOST_AUTO_TEST_CASE(reading_a_exp) {
        std::vector<std::string> text;
        text.push_back("12345.6789e-123");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_FLOAT, "12345.6789e-123");
    }

    BOOST_AUTO_TEST_CASE(reading_an_integer_then_identifier) {
        std::vector<std::string> text;
        text.push_back("1abc");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "1");
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "abc");
    }

    BOOST_AUTO_TEST_CASE(reading_invalid_identifier_then_integer_then_identifier) {
        std::vector<std::string> text;
        text.push_back("_1abc");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::INVALID, "_", 0, 0);
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "1", 0, 1);
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "abc", 0, 2);
    }

    BOOST_AUTO_TEST_CASE(reading_a_program_with_keywords) {
        std::vector<std::string> text;
        text.push_back("main() {");
        text.push_back("   a = 10;");
        text.push_back("   b = 20;");
        text.push_back("   c = a + b;");
        text.push_back("   write c;");
        text.push_back("}");

        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);
        // main() {
        checkTokenValueAndType(lexer.nextToken(), TokenType::KEYWORD_MAIN, "main", 0, 0);
        checkTokenValueAndType(lexer.nextToken(), TokenType::LEFT_PAREN, "(", 0, 4);
        checkTokenValueAndType(lexer.nextToken(), TokenType::RIGHT_PAREN, ")", 0, 5);
        checkTokenValueAndType(lexer.nextToken(), TokenType::LEFT_BRACE, "{", 0, 7);
        // a = 10;
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "a", 1, 3);
        checkTokenValueAndType(lexer.nextToken(), TokenType::ASSIGN, "=", 1, 5);
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "10", 1, 7);
        checkTokenValueAndType(lexer.nextToken(), TokenType::SEMICOLON, ";", 1, 9);
        // b = 20;
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "b", 2, 3);
        checkTokenValueAndType(lexer.nextToken(), TokenType::ASSIGN, "=", 2, 5);
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "20", 2, 7);
        checkTokenValueAndType(lexer.nextToken(), TokenType::SEMICOLON, ";", 2, 9);
        // c = a + b;
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "c", 3, 3);
        checkTokenValueAndType(lexer.nextToken(), TokenType::ASSIGN, "=", 3, 5);
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "a", 3, 7);
        checkTokenValueAndType(lexer.nextToken(), TokenType::PLUS, "+", 3, 9);
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "b", 3, 11);
        checkTokenValueAndType(lexer.nextToken(), TokenType::SEMICOLON, ";", 3, 12);
        // write c;
        checkTokenValueAndType(lexer.nextToken(), TokenType::KEYWORD_WRITE, "write", 4, 3);
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "c", 4, 9);
        checkTokenValueAndType(lexer.nextToken(), TokenType::SEMICOLON, ";", 4, 10);
        // }
        checkTokenValueAndType(lexer.nextToken(), TokenType::RIGHT_BRACE, "}", 5, 0);
    }

    BOOST_AUTO_TEST_CASE(reading_different_kind_of_comments) {
        std::vector<std::string> text;
        text.push_back("// helloworld");
        text.push_back("/* comment */");
        text.push_back("a  /* comment */");
        text.push_back("b");
        text.push_back("/* ");
        text.push_back(" comment */");
        text.push_back("c");
        text.push_back("d");
        text.push_back("e");
        text.push_back("// xxxx");
        text.push_back("f // xxxx");

        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::LINE_COMMENT, " helloworld");
        checkTokenValueAndType(lexer.nextToken(), TokenType::BLOCK_COMMENT, " comment ");
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "a");
        checkTokenValueAndType(lexer.nextToken(), TokenType::BLOCK_COMMENT, " comment ");
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "b");
        checkTokenValueAndType(lexer.nextToken(), TokenType::BLOCK_COMMENT, " \n comment ");
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "c");
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "d");
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "e");
        checkTokenValueAndType(lexer.nextToken(), TokenType::LINE_COMMENT, " xxxx");
        checkTokenValueAndType(lexer.nextToken(), TokenType::IDENT, "f");
        checkTokenValueAndType(lexer.nextToken(), TokenType::LINE_COMMENT, " xxxx");
    }

    BOOST_AUTO_TEST_CASE(error_lexer) {
        std::vector<std::string> text;
        text.push_back("$ ");
        text.push_back("@");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::INVALID, "$");
        checkTokenValueAndType(lexer.nextToken(), TokenType::INVALID, "@");
    }

/*
 100
 200
 00100 222.2
 0.0000001000
 0.00000010002
 0.001e222
 0.001e-2344
 0.001e+2344
 */
    BOOST_AUTO_TEST_CASE(reading_numbers) {
        std::vector<std::string> text;
        text.push_back("100 ");
        text.push_back("200");
        text.push_back("00100 222.2");
        text.push_back("0.0000001000");
        text.push_back("0.00000010002");
        text.push_back("0.001e222");
        text.push_back("0.001e-2344");
        text.push_back("0.001e+2344");

        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "100");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "200");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "0");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "0");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "100");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_FLOAT, "222.2");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_FLOAT, "0.0000001");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "0");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "0");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_INTEGER, "0");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_FLOAT, "0.00000010002");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_FLOAT, "0.001e222");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_FLOAT, "0.001e-2344");
        checkTokenValueAndType(lexer.nextToken(), TokenType::NUMBER_FLOAT, "0.001e+2344");
    }


    BOOST_AUTO_TEST_CASE(reading_invalid_identifier) {
        std::vector<std::string> text;
        text.push_back("_abc1");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        checkTokenValueAndType(lexer.nextToken(), TokenType::INVALID, "_abc1");
    }

    BOOST_AUTO_TEST_CASE(reading_an_integer) {
        std::vector<std::string> text;
        text.push_back("123");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        Token token = lexer.nextToken();

        checkTokenValueAndType(token, TokenType::NUMBER_INTEGER, "123");
    }

    BOOST_AUTO_TEST_CASE(reading_an_integer_disguise_as_float) {
        std::vector<std::string> text;
        text.push_back("123.e+m");
        std::vector<Token*> expectedValues {
                new Token(TokenType::NUMBER_INTEGER, "123", 0, 0),
                new Token(TokenType::DOT, ".", 0, 3),
                new Token(TokenType::IDENT, "e", 0, 4),
                new Token(TokenType::PLUS, "+", 0, 5),
                new Token(TokenType::IDENT, "m", 0, 6)
        };

        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        auto checkEquals = [&lexer](Token* expected) {
            Token token = lexer.nextToken();
            BOOST_CHECK_EQUAL(expected->value(), token.value());
            BOOST_CHECK(expected->type() == token.type());
            BOOST_CHECK(expected->location() == token.location());
        };

        std::for_each(expectedValues.begin(), expectedValues.end(), checkEquals);

        BOOST_CHECK(lexer.nextToken().type() == TokenType::END_OF_FILE);
    }

    BOOST_AUTO_TEST_CASE(reading_line_comment) {
        std::vector<std::string> text;
        text.push_back("//this is a comment");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        Token token = lexer.nextToken();

        BOOST_CHECK(token.type() == TokenType::LINE_COMMENT);
        BOOST_CHECK_EQUAL("this is a comment", token.value());
    }

    BOOST_AUTO_TEST_CASE(reading_multiline_comment) {
        std::vector<std::string> text;
        text.push_back("/* this is a ");
        text.push_back(" * multiline comment */");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        Token token = lexer.nextToken();

        BOOST_CHECK(token.type() == TokenType::BLOCK_COMMENT);
        BOOST_CHECK_EQUAL(" this is a \n * multiline comment ", token.value());
    }

    BOOST_AUTO_TEST_CASE(reading_simple_a_float) {
        std::vector<std::string> text;
        text.push_back("123.321");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        Token token = lexer.nextToken();

        BOOST_CHECK(token.type() == TokenType::NUMBER_FLOAT);
        BOOST_CHECK_EQUAL("123.321", token.value());
    }

    BOOST_AUTO_TEST_CASE(reading_a_float_with_exponential) {
        std::vector<std::string> text;
        text.push_back("123.0008e+312");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        Token token = lexer.nextToken();

        BOOST_CHECK(token.type() == TokenType::NUMBER_FLOAT);
        BOOST_CHECK_EQUAL("123.0008e+312", token.value());
    }

    BOOST_AUTO_TEST_CASE(reading_an_identifier) {
        std::vector<std::string> text;
        text.push_back("a1_23");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        Token token = lexer.nextToken();

        BOOST_CHECK(token.type() == TokenType::IDENT);
        BOOST_CHECK_EQUAL("a1_23", token.value());
    }

    BOOST_AUTO_TEST_CASE(reading_a_keyword) {
        std::vector<std::string> text;
        text.push_back("return");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        Token token = lexer.nextToken();

        BOOST_CHECK(token.type() == TokenType::KEYWORD_RETURN);
        BOOST_CHECK_EQUAL("return", token.value());
    }

    BOOST_AUTO_TEST_CASE(reading_stream_of_identifiers_and_keywords) {
        std::vector<std::string> text;
        text.push_back("a b_123 c12_ d if else return");
        text.push_back("e f ");
        InMemoryScanner inMemory(text);
        Lexer lexer(&inMemory);

        auto checkEquals = [&lexer](const std::string& n) {
            Token token = lexer.nextToken();
            TokenType maybeAKeywordToken = tokenTypeByKeyword(token.value());
            TokenType realTokenType = maybeAKeywordToken == TokenType::INVALID ? TokenType::IDENT : maybeAKeywordToken;

            BOOST_CHECK_EQUAL(n, token.value());
            BOOST_CHECK(token.type() == realTokenType);
        };

        for(auto i = text.begin(); i != text.end(); i++) {
            std::istringstream iss(*i);
            std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                             std::istream_iterator<std::string>());

            std::for_each(results.begin(), results.end(), checkEquals);
        }
    }

//
//BOOST_AUTO_TEST_CASE(reading_a_number) {
//    std::vector<std::string> text;
//    text.push_back("123");
//    InMemoryScanner inMemory(text);
//    Lexer lexer(&inMemory);
//
//    Token token = lexer.nextToken();
//
//    BOOST_CHECK(token.type() == TokenType::INTEGER);
//    BOOST_CHECK_EQUAL("123", token.value());
//}
//
//BOOST_AUTO_TEST_CASE(when_read_2_numbers_one_start_with_0_then_first_read_should_give_0_second_the_number) {
//    std::vector<std::string> text;
//    text.push_back("0123");
//    InMemoryScanner inMemory(text);
//    Lexer lexer(&inMemory);
//
//    Token zero = lexer.nextToken();
//    Token rest = lexer.nextToken();
//
//    BOOST_CHECK(zero.type() == TokenType::INTEGER);
//    BOOST_CHECK_EQUAL("0", zero.value());
//    BOOST_CHECK(rest.type() == TokenType::INTEGER);
//    BOOST_CHECK_EQUAL("123", rest.value());
//}
//
//BOOST_AUTO_TEST_CASE(reading_a_float_number) {
//    std::vector<std::string> text;
//    text.push_back("123.01230");
//    InMemoryScanner inMemory(text);
//    Lexer lexer(&inMemory);
//
//    Token token = lexer.nextToken();
//
//    BOOST_CHECK(token.type() == TokenType::FLOAT);
//    BOOST_CHECK_EQUAL("123.123", token.value());
//}

BOOST_AUTO_TEST_SUITE_END()