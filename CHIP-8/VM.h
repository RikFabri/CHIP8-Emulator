#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "OpCodeHandler.h"

namespace CHIP
{
    class VM
    {
    public:
        VM();
        ~VM();

        void Update();
        void LoadRom(const std::string& path);
        void ReloadRom();
        void ClearDisplay();

        const std::vector<uint8_t>& GetDisplay() const;
        bool DidDisplayUpdate() const;
        void PrintDisplay() const;

        void KeyDown(int key);
        void KeyUp(int key);

        int GetDisplayWidth() const;
        int GetDisplayHeight() const;
    private:
        // Addresses 0x050 to 0x0A0 contain all hex digits (0 - F)
        void LoadHexSpritesInMemory();
        // Merges two bytes into the right 16bit opcode
        uint16_t ConstructOpCode(uint8_t HighOrder, uint8_t LowOrder) const;

        int GetPixelIndex(int x, int y) const;

        // Converts a char in the 0-F range like 'a' into 0xA
        int ConvertCharToHex(char character) const;
    private:
        // A friend class that recognizes opcodes and implements the instructions
        friend class CHIP::OpCodeHandler;
        OpCodeHandler m_OpCodeHandler;

        bool m_DisplayUpdated;
        bool m_WaitForInput;
        int m_InputRegister;

        const static int m_DisplayWidth = 64;
        const static int m_DisplayHeight = 32;
    private:
        // Graphical memory
        // A pixel can only be on or off, represented by these uints being 1 or 0.
        // We can't use booleans since the vector template is specialized with bools and provides an annoying interface.
        std::vector<uint8_t> m_DisplayPixelValues;

        // Memory
        const static int MemorySize = 0x1000;
        uint8_t m_Memory[MemorySize];

        // Registers
        // General purpose
        const static int NrOfGeneralPurposeRegisters = 0x10;
        uint8_t Vx[NrOfGeneralPurposeRegisters];

        // Register for storing memory addresses
        uint16_t Vi;

        // Delay timer
        // This timer does nothing more than subtract 1 from the value of DT at a rate of 60Hz. When DT reaches 0, it deactivates.
        uint8_t Dt;

        // Sound timer
        // This timer decrements at a rate of 60Hz, however, as long as ST's value is greater than zero, the Chip-8 buzzer will sound.
        uint8_t St;

        // Program counter
        uint16_t Pc;

        // Stack pointer
        uint8_t Sp;

        // Stack
        uint16_t m_Stack[0x10];

        // State of all supported keys
        std::map<uint8_t, bool> m_KeyboardState;
    };
}
