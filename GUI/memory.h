#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

class Memory {
private:
    string memory[256];
    int size = 256;

public:
    Memory();
    string getCell(int address) const;
    void setCell(int address, const string& value);
    void displayMemory() const;
};

#endif // MEMORY_H
