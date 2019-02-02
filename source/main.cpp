#include <iostream>
#include <vector>
#include <fstream>
#include "Token.h"
#include "InMemoryScanner.h"
#include "Lexer.h"

int main(int argc, char** argv) {
    std::vector<std::string> text;
    //Read the file line by line and put them into the vector
    std::ifstream file(argv[1]);
    std::ofstream outfile("../../resources/output_file.txt");
    if(file.fail()){
        perror("File: does not exist");
        exit(1);
    }

    std::string str;
    while (std::getline(file, str)) {
        text.push_back(str);
    }

    InMemoryScanner inMemory(text);
    Lexer lexer(&inMemory);
    Token token = lexer.nextToken();
    bool first = true;
    while(token.type() != TokenType::END_OF_FILE) {
        std::cout<<token<<std::endl;
        if(first) {
            first = false;
        } else {
            outfile<<" ";
        }
        outfile<<token.toString();
        token = lexer.nextToken();
    }
    outfile.close();
    return 0;
}