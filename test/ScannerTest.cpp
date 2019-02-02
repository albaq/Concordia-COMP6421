//
// Created by Alvaro Tolosa on 2019-01-30.
//

#define BOOST_TEST_MODULE Main

#include <boost/test/unit_test.hpp>
#include "../source/InMemoryScanner.h"

BOOST_AUTO_TEST_SUITE(scanner_suite)

BOOST_AUTO_TEST_CASE(when_no_text_then_nextChar_returns_EOF) {
        std::vector<std::string> text;

        InMemoryScanner inMemory(text);

        BOOST_CHECK_EQUAL(0, inMemory.nextChar());
}

BOOST_AUTO_TEST_CASE(when_text_has_just_one_empty_string_then_nextChar_returns_EOF) {
        std::vector<std::string> text;
        text.push_back("");

        InMemoryScanner inMemory(text);

        BOOST_CHECK_EQUAL(0, inMemory.nextChar());
}

BOOST_AUTO_TEST_CASE(all_text_should_be_read) {
        std::vector<std::string> text;
        std::string last_line = "another_id";
        text.push_back("");
        text.push_back("");
        text.push_back(last_line);
        InMemoryScanner inMemory(text);
        auto checkEquals = [&inMemory](const char& n) {
            BOOST_CHECK_EQUAL(n, inMemory.nextChar());
        };

        BOOST_CHECK_EQUAL('\n', inMemory.nextChar());
        BOOST_CHECK_EQUAL('\n', inMemory.nextChar());
        std::for_each(last_line.begin(), last_line.end(), checkEquals);
        BOOST_CHECK_EQUAL(0, inMemory.nextChar());
}

BOOST_AUTO_TEST_CASE(when_reading_a_char_and_backing_up_same_char_should_be_read) {
        std::vector<std::string> text;
        text.push_back("ab");

        InMemoryScanner inMemory(text);

        BOOST_CHECK_EQUAL('a', inMemory.nextChar());
        inMemory.backupChar();
        BOOST_CHECK_EQUAL('a', inMemory.nextChar());
}

BOOST_AUTO_TEST_CASE(when_reading_a_char_consumig_a_line_and_backing_up_same_char_should_be_read) {
        std::vector<std::string> text;
        text.push_back("a");
        text.push_back("b");
        InMemoryScanner inMemory(text);

        BOOST_CHECK_EQUAL('a', inMemory.nextChar());
        BOOST_CHECK_EQUAL('\n', inMemory.nextChar());
        BOOST_CHECK_EQUAL('b', inMemory.nextChar());
        inMemory.backupChar();
        BOOST_CHECK_EQUAL('b', inMemory.nextChar());
}

BOOST_AUTO_TEST_CASE(when_in_EOF_then_backup_and_read_last_char_should_be_returned) {
        std::vector<std::string> text;
        text.push_back("ab");
        InMemoryScanner inMemory(text);

        BOOST_CHECK_EQUAL('a', inMemory.nextChar());
        BOOST_CHECK_EQUAL('b', inMemory.nextChar());
        BOOST_CHECK_EQUAL(0, inMemory.nextChar());
        inMemory.backupChar();
        BOOST_CHECK_EQUAL('b', inMemory.nextChar());
}

BOOST_AUTO_TEST_SUITE_END()