#include "machine.h"
#include <iostream>
#include <algorithm>

Machine::Machine() {}

void Machine::loadProgramFromText(const string& instruction) {
    string trimmedInstruction = instruction;
    trimmedInstruction.erase(remove_if(trimmedInstruction.begin(), trimmedInstruction.end(), ::isspace), trimmedInstruction.end());
    transform(trimmedInstruction.begin(), trimmedInstruction.end(), trimmedInstruction.begin(), ::toupper);

    memory.setCell(instructionCount, trimmedInstruction);
    instructionCount++;
}

void Machine::executeProgram() {
    for (int i = 0; i < instructionCount; ++i) {
        if (!processor.executeNextInstruction(memory)) {
            break;
        }
        displayState();
    }
}

void Machine::displayState() const {
    cout << "\n=== Machine State ===\n";
    cout << "Program Counter: 0x" << hex << uppercase << processor.getProgramCounter() << dec << endl;
    cout << "Instruction Register: " << processor.getInstructionRegister() << endl;
    processor.getRegisterBank().displayRegisters();
    memory.displayMemory();
}

void Machine::resetMemory() {
    for (int i = 0; i < 256; ++i) {
        memory.setCell(i, "0000");
    }
}

void Machine::resetRegisters() {
    for (int i = 0; i < 16; ++i) {
        processor.getRegisterBank().displayRegisters();
    }
}
