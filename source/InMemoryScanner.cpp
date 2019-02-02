//
// Created by Alvaro Tolosa on 2019-01-29.
//

#include "InMemoryScanner.h"

InMemoryScanner::InMemoryScanner(std::vector<std::string> text): text(text) {
    actualRow = 0;
    readPosition = 0;
    position = 0;
}

char InMemoryScanner::nextChar() {
    char response = 0;

    if(endOfFile()) return 0;

    if (readPosition < text[actualRow].length()) {
        response = text[actualRow][readPosition];
        position = readPosition;
        readPosition++;
    } else if (actualRow < text.size()) {
        response = '\n';
        readPosition = 0;
        position = readPosition;
        actualRow++;
    }

    return response;
}

void InMemoryScanner::backupChar() {
    if(text.empty() || (actualRow <= 0 && readPosition <= 0)) return;

    if(readPosition > 0) readPosition--;
    else if (actualRow > 0) {
        actualRow--;
        readPosition = text[actualRow].empty()? 0: (int)text[actualRow].size();
    }
    position = readPosition;
}

std::pair<int, int> InMemoryScanner::location() {
    return std::pair<int, int>(actualRow, position);
}
