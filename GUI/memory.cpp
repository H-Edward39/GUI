#include "memory.h"
#include <algorithm>

Memory::Memory() {
    for (int i = 0; i < size; i++) {
        memory[i] = "0000";
    }
}

string Memory::getCell(int address) const {
    if (address >= 0 && address < size) {
        return memory[address];
    }
    throw runtime_error("Memory address out of bounds: " + to_string(address));
}

void Memory::setCell(int address, const string& value) {
    if (address >= 0 && address < size) {
        string paddedValue = value;
        while (paddedValue.length() < 4) paddedValue = "0" + paddedValue;
        memory[address] = paddedValue.substr(0, 4);
    } else {
        throw runtime_error("Memory address out of bounds: " + to_string(address));
    }
}

void Memory::displayMemory() const {
    cout << "\nMemory Contents:\n";
    cout << "Addr\t+0\t+1\t+2\t+3\t+4\t+5\t+6\t+7\n";
    for (int i = 0; i < size; i += 8) {
        cout << hex << uppercase << setw(3) << setfill('0') << i << "\t";
        for (int j = 0; j < 8 && (i + j) < size; ++j) {
            cout << memory[i + j] << "\t";
        }
        cout << endl;
    }
    cout << dec;
}
