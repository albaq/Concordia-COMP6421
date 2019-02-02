//
// Created by Alvaro Tolosa on 2019-01-29.
//

#ifndef SIMPLECOMPILER_INMEMORYSCANNER_H
#define SIMPLECOMPILER_INMEMORYSCANNER_H

#include <vector>
#include <string>
#include "CharacterScanner.h"

class InMemoryScanner : public CharacterScanner {
    std::vector<std::string> text;
    int actualRow;
    int position;
    int readPosition;

    bool endOfFile() {
        return text.empty() || (readPosition >= text[actualRow].length() && actualRow + 1 >= text.size());
    }
public:
    InMemoryScanner(std::vector<std::string>);

    char nextChar();
    void backupChar();
    std::pair<int, int> location();
};


#endif //SIMPLECOMPILER_INMEMORYSCANNER_H
