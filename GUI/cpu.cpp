#include "cpu.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

// Register class implementation
Register::Register() {
    fill(begin(registers), end(registers), 0);
}

int Register::getCell(int address) const {
    if (address >= 0 && address < 16) return registers[address];
    throw runtime_error("Register index out of bounds");
}

void Register::setCell(int address, int value) {
    if (address >= 0 && address < 16) registers[address] = value;
    else throw runtime_error("Register index out of bounds");
}

void Register::displayRegisters() const {
    cout << "\nRegister Contents:\n";
    for (int i = 0; i < 16; i++) {
        cout << "R" << hex << i << ": 0x"
                  << setw(4) << setfill('0') << registers[i] << dec << endl;
    }
}

// ALU class implementation
int ALU::hexToDec(const string& hexStr) {
    return stoi(hexStr, nullptr, 16);
}

string ALU::decToHex(int value) {
    stringstream ss;
    ss << hex << setw(4) << setfill('0') << value;
    return ss.str();
}

bool ALU::isValidHex(const string& str) {
    return all_of(str.begin(), str.end(), ::isxdigit);
}

int ALU::add(int a, int b) {
    return a + b;
}

// CU class implementation
void CU::rxy(char opcode, int regIdx, int memAddr, Register& reg, Memory& mem, int& pc) {
    switch (opcode) {
    case '1':
        reg.setCell(regIdx, ALU::hexToDec(mem.getCell(memAddr)));
        break;
    case '2':
        reg.setCell(regIdx, memAddr & 0xFF);
        break;
    case '3':
        mem.setCell(memAddr, ALU::decToHex(reg.getCell(regIdx)));
        break;
    case 'B':
        if (reg.getCell(regIdx) == 0) pc = memAddr - 1;
        break;
    default:
        cout << "Unhandled RXY opcode: " << opcode << endl;
    }
}

void CU::rst(char opcode, int reg1, int reg2, int destReg, Register& reg) {
    if (opcode == '5') reg.setCell(destReg, ALU::add(reg.getCell(reg1), reg.getCell(reg2)));
    else if (opcode == '6') cout << "Result: " << reg.getCell(reg1) << endl;
}

// CPU class implementation
CPU::CPU() {}

int CPU::getProgramCounter() const { return programCounter; }

string CPU::getInstructionRegister() const { return instructionRegister; }

const Register& CPU::getRegisterBank() const { return registerBank; }

bool CPU::executeNextInstruction(Memory& memory) {
    instructionRegister = memory.getCell(programCounter);
    if (instructionRegister == "C000") return false;

    char opcode = instructionRegister[0];
    string operand = instructionRegister.substr(1, 3);
    int addr = stoi(operand, nullptr, 16);

    if (opcode == '1' || opcode == '2' || opcode == '3' || opcode == 'B') {
        int regIdx = (addr >> 8) & 0xF;
        int memAddr = addr & 0xFF;
        controlUnit.rxy(opcode, regIdx, memAddr, registerBank, memory, programCounter);
    } else if (opcode == '5' || opcode == '6') {
        int reg1 = (addr >> 8) & 0xF;
        int reg2 = (addr >> 4) & 0xF;
        int destReg = addr & 0xF;
        controlUnit.rst(opcode, reg1, reg2, destReg, registerBank);
    }
    programCounter++;
    return true;
}
