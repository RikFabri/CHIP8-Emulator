#pragma once
#include <functional>
#include <vector>
#include <cstdint>

namespace CHIP
{
    class VM;

    // **A friend class to VM**
    // to recognizes opcodes and implement the instructions
    class OpCodeHandler
    {
        struct OpCodeDetails
        {
            const uint16_t opcode;
            const uint16_t mask;
            const std::function<void(OpCodeHandler*, VM&, uint16_t)> instruction;
        };
        
    public:
        OpCodeHandler(bool printInstructions = false);
        void HandleOpCode(VM& vm, uint16_t opcode);
    private:
        // Returns the value of X in a hex value of NXNN
        uint8_t ExtractX(uint16_t opcode);
        // Returns the value of NNN in a hex value of XNNN
        uint16_t ExtractNNN(uint16_t opcode);
        // Returns the value of NN in a hex value of XXNN
        uint8_t ExtractNN(uint16_t opcode);
        // Returns the value of N in a hex value of XXXN
        uint8_t ExtractN(uint16_t opcode);
        // Returns the value of Y in a hex value of NNYN
        uint8_t ExtractY(uint16_t opcode);
    private:
        bool m_PrintInstruction;
        std::vector<OpCodeDetails> m_OpCodeDetails;
    
        // Instructions
        // These are member functions so that we don't have to friend all of them separately in VM

        // Jump to address NNN
        void Opcode1NNN(VM& vm, uint16_t opcode);
        // Sets Vx to NN
        void Opcode6XNN(VM& vm, uint16_t opcode);
        // Sets register I to NNN
        void OpcodeANNN(VM& vm, uint16_t opcode);
        // Draw
        void OpcodeDXYN(VM& vm, uint16_t opcode);
        // Adds NN to Vx
        void Opcode7XNN(VM& vm, uint16_t opcode);
        // Adds Vx to I
        void OpcodeFX1E(VM& vm, uint16_t opcode);
        // Skips the next instruction if Vx equals NN
        void Opcode3XNN(VM& vm, uint16_t opcode);
        // Sets VX to the value of the delay timer
        void OpcodeFX07(VM& vm, uint16_t opcode);
        // Calls subroutine at NNN
        void Opcode2NNN(VM& vm, uint16_t opcode);
        // Sets the delay timer to VX
        void OpcodeFX15(VM& vm, uint16_t opcode);
        // Sets VX to the value of VY.
        void Opcode8XY0(VM& vm, uint16_t opcode);
        // Skips the next instruction if the key stored in VX is not pressed
        void OpcodeEXA1(VM& vm, uint16_t opcode);
        // Clear display
        void Opcode00E0(VM& vm, uint16_t opcode);
        // Returns from subroutine
        void Opcode00EE(VM& vm, uint16_t opcode);
        // Sets VX to VX and VY. (Bitwise AND operation)
        void Opcode8XY2(VM& vm, uint16_t opcode);
        // Stores the least significant bit of VX in VF and then shifts VX to the right by 1
        void Opcode8XY6(VM& vm, uint16_t opcode);
        // Skips the next instruction if the key stored in VX is pressed
        void OpcodeEX9E(VM& vm, uint16_t opcode);
        // Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not
        void Opcode8XY4(VM& vm, uint16_t opcode);
        // A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event);
        void OpcodeFX0A(VM& vm, uint16_t opcode);
        // Fills V0 to VX (including VX) with values from memory starting at address I.
        // The offset from I is increased by 1 for each value written, but I itself is left unmodified
        void OpcodeFX65(VM& vm, uint16_t opcode);
        // Stores the binary-coded decimal representation of VX, 
        // with the most significant of three digits at the address in I, 
        // the middle digit at I plus 1, and the least significant digit at I plus 2. 
        // (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, 
        // the tens digit at location I+1, and the ones digit at location I+2.);
        void OpcodeFX33(VM& vm, uint16_t opcode);
        // Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
        void OpcodeFX29(VM& vm, uint16_t opcode);
        // Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
        void OpcodeCXNN(VM& vm, uint16_t opcode);
        // Skips the next instruction if VX does not equal NN
        void Opcode4XNN(VM& vm, uint16_t opcode);
        // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not
        void Opcode8XY5(VM& vm, uint16_t opcode);
        // Sets the sound timer to VX
        void OpcodeFX18(VM& vm, uint16_t opcode);
        // Stores V0 to VX (including VX) in memory starting at address I. 
        // The offset from I is increased by 1 for each value written, but I itself is left unmodified
        void OpcodeFX55(VM& vm, uint16_t opcode);
        // Sets VX to VX xor VY
        void Opcode8XY3(VM& vm, uint16_t opcode);
        // Skips the next instruction if VX does not equal VY
        void Opcode9XY0(VM& vm, uint16_t opcode);
        // Skips the next instruction if VX equals VY
        void Opcode5XY0(VM& vm, uint16_t opcode);
        // Stores the most significant bit of VX in VF and then shifts VX to the left by 1
        void Opcode8XYE(VM& vm, uint16_t opcode);
        // Sets VX to VX or VY. (Bitwise OR operation)
        void Opcode8XY1(VM& vm, uint16_t opcode);
        // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not
        void Opcode8XY7(VM& vm, uint16_t opcode);
        // Jumps to the address NNN plus V0
        void OpcodeBNNN(VM& vm, uint16_t opcode);
    };
}
