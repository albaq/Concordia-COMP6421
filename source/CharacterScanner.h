//
// Created by Alvaro Tolosa on 2019-01-29.
//

#ifndef SIMPLECOMPILER_CHARACTERSCANNER_H
#define SIMPLECOMPILER_CHARACTERSCANNER_H

#include <utility>

class CharacterScanner {
public:
    virtual char nextChar() = 0;
    virtual void backupChar() = 0;
    virtual std::pair<int, int> location() = 0;
    virtual ~CharacterScanner() {}
};

#endif //SIMPLECOMPILER_CHARACTERSCANNER_H
