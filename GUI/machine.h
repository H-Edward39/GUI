#ifndef MACHINE_H
#define MACHINE_H

#include "cpu.h"
#include "memory.h"

using namespace std;

class Machine {
private:
    CPU processor;
    Memory memory;
    int instructionCount = 0;

public:
    Machine();
    void loadProgramFromText(const string& instruction);
    void executeProgram();
    void displayState() const;
    void resetMemory();
    void resetRegisters();
    const Register& getRegisterBank() const { return processor.getRegisterBank(); }
    string getMemoryCell(int address) const { return memory.getCell(address); }
};

#endif // MACHINE_H
