#ifndef CPU_H
#define CPU_H

#include "memory.h"
#include <string>

using namespace std;

class Register {
private:
    int registers[16];

public:
    Register();
    int getCell(int address) const;
    void setCell(int address, int value);
    void displayRegisters() const;
};

class ALU {
public:
    static int hexToDec(const string& hexStr);
    static string decToHex(int value);
    static bool isValidHex(const string& str);
    static int add(int a, int b);
};

class CU {
public:
    void rxy(char opcode, int regIdx, int memAddr, Register& reg, Memory& mem, int& pc);
    void rst(char opcode, int reg1, int reg2, int destReg, Register& reg);
};

class CPU {
private:
    int programCounter = 0;
    string instructionRegister;
    Register registerBank;
    CU controlUnit;

public:
    CPU();
    int getProgramCounter() const;
    string getInstructionRegister() const;
    const Register& getRegisterBank() const;
    bool executeNextInstruction(Memory& memory);
};

#endif // CPU_H
