cmake_minimum_required(VERSION 3.13)

#project(BoostTest)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

set(Boost_INCLUDE_DIR /usr/local/Cellar/boost/1.68.0/include)
set(Boost_LIBRARY_DIR /usr/local/Cellar/boost/1.68.0/lib)

include_directories (${SimpleCompiler_SOURCE_DIR}/source)
include_directories(${Boost_INCLUDE_DIRS})

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

add_executable (test ScannerTest.cpp LexerTest.cpp ${SimpleCompiler_SOURCE_DIR}/source/InMemoryScanner.cpp ${SimpleCompiler_SOURCE_DIR}/source/Lexer.cpp ${SimpleCompiler_SOURCE_DIR}/source/Token.cpp)

target_link_libraries(test
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )

enable_testing()
add_test(MyTest test)

#message(STATUS "start running cmake...")