#include "OpCodeHandler.h"
#include "VM.h"
#include <thread>

CHIP::OpCodeHandler::OpCodeHandler(bool printInstructions)
    : m_OpCodeDetails
    {
        { 0x00E0, 0xFFFF, &OpCodeHandler::Opcode00E0 },
        { 0x00EE, 0xFFFF, &OpCodeHandler::Opcode00EE },
        { 0x1000, 0xF000, &OpCodeHandler::Opcode1NNN },
        { 0x2000, 0xF000, &OpCodeHandler::Opcode2NNN },
        { 0x3000, 0xF000, &OpCodeHandler::Opcode3XNN },
        { 0x4000, 0xF000, &OpCodeHandler::Opcode4XNN },
        { 0x5000, 0xF00F, &OpCodeHandler::Opcode5XY0 },
        { 0x6000, 0xF000, &OpCodeHandler::Opcode6XNN },
        { 0x7000, 0xF000, &OpCodeHandler::Opcode7XNN },
        { 0x8000, 0xF00F, &OpCodeHandler::Opcode8XY0 },
        { 0x8001, 0xF00F, &OpCodeHandler::Opcode8XY1 },
        { 0x8002, 0xF00F, &OpCodeHandler::Opcode8XY2 },
        { 0x8003, 0xF00F, &OpCodeHandler::Opcode8XY3 },
        { 0x8004, 0xF00F, &OpCodeHandler::Opcode8XY4 },
        { 0x8005, 0xF00F, &OpCodeHandler::Opcode8XY5 },
        { 0x8006, 0xF00F, &OpCodeHandler::Opcode8XY6 },
        { 0x8007, 0xF00F, &OpCodeHandler::Opcode8XY7 },
        { 0x800E, 0xF00F, &OpCodeHandler::Opcode8XYE },
        { 0x9000, 0xF00F, &OpCodeHandler::Opcode9XY0 },
        { 0xA000, 0xF000, &OpCodeHandler::OpcodeANNN },
        { 0xB000, 0xF000, &OpCodeHandler::OpcodeBNNN },
        { 0xC000, 0xF000, &OpCodeHandler::OpcodeCXNN },
        { 0xD000, 0xF000, &OpCodeHandler::OpcodeDXYN },
        { 0xE09E, 0xF0FF, &OpCodeHandler::OpcodeEX9E },
        { 0xE0A1, 0xF0FF, &OpCodeHandler::OpcodeEXA1 },
        { 0xF007, 0xF0FF, &OpCodeHandler::OpcodeFX07 },
        { 0xF00A, 0xF0FF, &OpCodeHandler::OpcodeFX0A },
        { 0xF015, 0xF0FF, &OpCodeHandler::OpcodeFX15 },
        { 0xF018, 0xF0FF, &OpCodeHandler::OpcodeFX18 },
        { 0xF01E, 0xF0FF, &OpCodeHandler::OpcodeFX1E },
        { 0xF029, 0xF0FF, &OpCodeHandler::OpcodeFX29 },
        { 0xF033, 0xF0FF, &OpCodeHandler::OpcodeFX33 },
        { 0xF055, 0xF0FF, &OpCodeHandler::OpcodeFX55 },
        { 0xF065, 0xF0FF, &OpCodeHandler::OpcodeFX65 }
    }
    , m_PrintInstruction(printInstructions)
{
}

void CHIP::OpCodeHandler::HandleOpCode(VM& vm, uint16_t opcode)
{
    for (auto details : m_OpCodeDetails)
    {
        if ((details.mask & opcode) == details.opcode)
        {
            details.instruction(this, vm, opcode);
            return;
        }
    }

    std::cout << "Opcode " << std::hex << opcode << " not implemented" << std::endl;
}

void CHIP::OpCodeHandler::Opcode1NNN(VM& vm, uint16_t opcode)
{
    uint16_t address = ExtractNNN(opcode);
    vm.Pc = address;

    if(m_PrintInstruction)
    {
        std::cout << "Jump to address: " << std::hex << address << std::endl;
        std::cout << "This is address " << std::hex << address - 0x200 << " in the rom" << std::endl;
    }
}

void CHIP::OpCodeHandler::Opcode6XNN(VM& vm, uint16_t opcode)
{
    auto X = ExtractX(opcode);
    auto NN = opcode & 0x00FF;

    vm.Vx[X] = NN;

    if(m_PrintInstruction)
        std::cout << "Sets V" << X << " to: " << std::hex << NN << std::endl;
}

void CHIP::OpCodeHandler::OpcodeANNN(VM& vm, uint16_t opcode)
{
    uint16_t NNN = ExtractNNN(opcode);
    vm.Vi = NNN;

    if(m_PrintInstruction)
        std::cout << "Set register I to: " << std::hex << NNN << std::endl;
}

void CHIP::OpCodeHandler::OpcodeDXYN(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto y = ExtractY(opcode);
    auto n = opcode & 0x000F;

    // Top left position of the sprite
    auto xSpriteCoord = vm.Vx[x];
    auto ySpriteCoord = vm.Vx[y];

    bool collision = false;

    // The sprite is rendered 8 pixels wide (a pixel per bit) and n bytes high
    for (int height = 0; height < n; ++height)
    {
        for (int width = 0; width < 8; ++width)
        {
            const auto address = vm.Vi;
            const auto value = vm.m_Memory[address + height];

            // Create mask to see if the relevant bit is on
            const auto mask = (0b10000000 >> width);
            const auto isPixelOnInSprite = (value & mask) > 0;

            if (!isPixelOnInSprite)
                continue;

            // If we need to draw this pixel, we xor it onto the display, setting a collision flag if we undid an older pixel
            const auto pixelIndex = vm.GetPixelIndex(xSpriteCoord + width, ySpriteCoord + height);

            if (vm.m_DisplayPixelValues[pixelIndex] == 1)
            {
                vm.m_DisplayPixelValues[pixelIndex] = 0;
                collision = true;
                continue;
            }

            vm.m_DisplayPixelValues[pixelIndex] = 1;
        }
    }

    vm.Vx[0x0F] = collision ? 1 : 0;
    vm.m_DisplayUpdated = true;

    if (m_PrintInstruction)
        std::cout << "drawcall" << std::endl;
}

void CHIP::OpCodeHandler::Opcode7XNN(VM& vm, uint16_t opcode)
{
    auto X = ExtractX(opcode);
    auto NN = opcode & 0x00FF;

    vm.Vx[X] += NN;

    if(m_PrintInstruction)
        std::cout << "Add " << std::hex << NN << " to V" << X << "\nmaking " << std::hex << (unsigned)vm.Vx[X] << std::endl;
}

void CHIP::OpCodeHandler::OpcodeFX1E(VM& vm, uint16_t opcode)
{
    auto X = ExtractX(opcode);

    vm.Vi += vm.Vx[X];

    if(m_PrintInstruction)
        std::cout << "Adding V" << X << " to Vi making " << std::hex << vm.Vi << std::endl;
}

void CHIP::OpCodeHandler::Opcode3XNN(VM& vm, uint16_t opcode)
{
    auto X = ExtractX(opcode);
    auto NN = ExtractNN(opcode);

    if(vm.Vx[X] == NN)
        vm.Pc += 2;

    if(m_PrintInstruction)
        std::cout << "Skipping instruction if " << (unsigned)vm.Vx[X] << " == " << NN << std::endl;
}

void CHIP::OpCodeHandler::OpcodeFX07(VM& vm, uint16_t opcode)
{
    auto X = ExtractX(opcode);

    vm.Vx[X] = vm.Dt;

    if(m_PrintInstruction)
        std::cout << "Set V" << X << " to delay timer which is " << (unsigned)vm.Dt << std::endl;
}

void CHIP::OpCodeHandler::Opcode2NNN(VM& vm, uint16_t opcode)
{
    uint16_t NNN = ExtractNNN(opcode);

    vm.m_Stack[vm.Sp++] = vm.Pc;
    vm.Pc = NNN;

    if(m_PrintInstruction)
        std::cout << "Called subroutine at " << NNN << std::endl;
}

void CHIP::OpCodeHandler::OpcodeFX15(VM& vm, uint16_t opcode)
{
    auto X = ExtractX(opcode);

    vm.Dt = vm.Vx[X];

    if(m_PrintInstruction)
        std::cout << "Set delay timer to V" << X << std::endl;
}

void CHIP::OpCodeHandler::Opcode8XY0(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto y = ExtractY(opcode);

    vm.Vx[x] = vm.Vx[y];

    if (m_PrintInstruction)
        std::cout << "Set V" << x << " to V" << y << std::endl;
}

void CHIP::OpCodeHandler::OpcodeEXA1(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto val = vm.Vx[x];

    if (!vm.m_KeyboardState[val])
        vm.Pc += 2;

    if (m_PrintInstruction)
        std::cout << "Skip next instruction if " << val << " isn't pressed" << std::endl;
}

void CHIP::OpCodeHandler::Opcode00E0(VM& vm, uint16_t opcode)
{
    vm.ClearDisplay();

    if (m_PrintInstruction)
        std::cout << "Clear display" << std::endl;
}

void CHIP::OpCodeHandler::Opcode00EE(VM& vm, uint16_t opcode)
{
    vm.Pc = vm.m_Stack[--vm.Sp];

    if (m_PrintInstruction)
        std::cout << "Return from subroutine, jumped back to " << std::hex << vm.m_Stack[vm.Sp] << std::endl;
}

void CHIP::OpCodeHandler::Opcode8XY2(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto y = ExtractY(opcode);

    vm.Vx[x] = vm.Vx[x] & vm.Vx[y];

    if (m_PrintInstruction)
        std::cout << "Set V" << x << " to the bitwise and of itself and V" << y << std::endl;
}

void CHIP::OpCodeHandler::Opcode8XY6(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);

    vm.Vx[0x0F] = vm.Vx[x] & 0x01;
    vm.Vx[x] >>= 1;

    if (m_PrintInstruction)
        std::cout << "Stored least significant bit of V" << x << " in Vf and then bitshifted it >> by one" << std::endl;
}

void CHIP::OpCodeHandler::OpcodeEX9E(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto val = vm.Vx[x];

    if (vm.m_KeyboardState[val])
        vm.Pc += 2;

    if (m_PrintInstruction)
        std::cout << "Skip next instruction if " << val << " is pressed" << std::endl;
}

void CHIP::OpCodeHandler::Opcode8XY4(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto y = ExtractY(opcode);

    int sum = vm.Vx[x] + vm.Vx[y];

    vm.Vx[0x0F] = sum > 0x00FF ? 1 : 0;

    vm.Vx[x] = uint8_t(sum & 0x00FF);

    if (m_PrintInstruction)
        std::cout << "Added V" << y << " to V" << x << " and set carry flag" << std::endl;
}

void CHIP::OpCodeHandler::OpcodeFX0A(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);

    vm.m_WaitForInput = true;
    vm.m_InputRegister = x;

    if (m_PrintInstruction)
        std::cout << "Waiting for input and saving it in V" << x << std::endl;
}

void CHIP::OpCodeHandler::OpcodeFX65(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    
    for (int i = 0; i <= x; ++i)
    {
        vm.Vx[i] = vm.m_Memory[vm.Vi + i];
    }

    if (m_PrintInstruction)
        std::cout << "Load data starting at address " << vm.Vi << " into V0 to V" << x << std::endl;
}

void CHIP::OpCodeHandler::OpcodeFX33(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto value = vm.Vx[x];

    vm.m_Memory[vm.Vi]      = value / 100;
    vm.m_Memory[vm.Vi + 1]  = int(value / 10) % 10;
    vm.m_Memory[vm.Vi + 2]  = value % 10;

    if (m_PrintInstruction)
        std::cout << "Saved the decimal value of " << std::hex << value << " into memory starting at " << std::hex << (unsigned)vm.Vi << std::endl;
}

void CHIP::OpCodeHandler::OpcodeFX29(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);

    const uint8_t fontHeight = 5;
    vm.Vi = 0x050 + (vm.Vx[x] * fontHeight);

    if (m_PrintInstruction)
        std::cout << "Set Vi to the address of sprite " << std::hex << x << std::endl;
}

void CHIP::OpCodeHandler::OpcodeCXNN(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto nn = ExtractNN(opcode);

    vm.Vx[x] = uint8_t(std::rand() % 256) & nn;

    if (m_PrintInstruction)
        std::cout << "Set V" << x << " to a random value & " << (unsigned)nn << " making " << (unsigned)vm.Vx[x] << std::endl;
}

void CHIP::OpCodeHandler::Opcode4XNN(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto nn = ExtractNN(opcode);

    if (vm.Vx[x] != nn)
        vm.Pc += 2;

    if (m_PrintInstruction)
        std::cout << "Skip instruction if " << vm.Vx[x] << " != " << nn << std::endl;
}

void CHIP::OpCodeHandler::Opcode8XY5(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto y = ExtractY(opcode);

    vm.Vx[0x0F] = vm.Vx[x] > vm.Vx[y] ? 1 : 0;

    vm.Vx[x] -= vm.Vx[y];

    if (m_PrintInstruction)
        std::cout << "Subtracted V" << y << " from V" << x << " and set borrow flag Vf" << std::endl;
}

void CHIP::OpCodeHandler::OpcodeFX18(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);

    vm.St = vm.Vx[x];

    if (m_PrintInstruction)
        std::cout << "Set sound timer to " << (unsigned)vm.St << std::endl;
}

void CHIP::OpCodeHandler::OpcodeFX55(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);

    for (int i = 0; i <= x; ++i)
    {
        vm.m_Memory[vm.Vi + i] = vm.Vx[i];
    }

    if (m_PrintInstruction)
        std::cout << "Load data from V0 to V" << x << " into memory starting at " << vm.Vi << std::endl;
}

void CHIP::OpCodeHandler::Opcode8XY3(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto y = ExtractY(opcode);

    vm.Vx[x] = vm.Vx[x] ^ vm.Vx[y];

    if (m_PrintInstruction)
        std::cout << "Set V" << x << " to itself xor " << (unsigned)vm.Vx[y] << std::endl;
}

void CHIP::OpCodeHandler::Opcode9XY0(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto y = ExtractY(opcode);

    if (vm.Vx[x] != vm.Vx[y])
        vm.Pc += 2;

    if (m_PrintInstruction)
        std::cout << "Skip instruction if " << (unsigned)vm.Vx[x] << "!=" << (unsigned)vm.Vx[y] << std::endl;
}

void CHIP::OpCodeHandler::Opcode5XY0(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto y = ExtractY(opcode);

    if (vm.Vx[x] == vm.Vx[y])
        vm.Pc += 2;

    if (m_PrintInstruction)
        std::cout << "Skips instruction if " << (unsigned)vm.Vx[x] << "==" << (unsigned)vm.Vx[y] << std::endl;
}

void CHIP::OpCodeHandler::Opcode8XYE(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto y = ExtractY(opcode);

    vm.Vx[0x0F] = (vm.Vx[x] & 0b10000000);
    vm.Vx[x] <<= 1;

    if (m_PrintInstruction)
        std::cout << "Stored most significant bit in Vf and bitshifted V" << x << " to the left by one" << std::endl;
}

void CHIP::OpCodeHandler::Opcode8XY1(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto y = ExtractY(opcode);

    vm.Vx[x] |= vm.Vx[y];

    if (m_PrintInstruction)
        std::cout << "sets Vx to Vx or Vy" << std::endl;
}

void CHIP::OpCodeHandler::Opcode8XY7(VM& vm, uint16_t opcode)
{
    auto x = ExtractX(opcode);
    auto y = ExtractY(opcode);

    auto vx = vm.Vx[x];
    auto vy = vm.Vx[y];

    vm.Vx[0x0F] = vx > vy ? 0 : 1;

    vm.Vx[x] = vy - vx;

    if (m_PrintInstruction)
        std::cout << "Subtracted Vx from Vy and stored in Vx, set Vf if borrowed" << std::endl;
}

void CHIP::OpCodeHandler::OpcodeBNNN(VM& vm, uint16_t opcode)
{
    auto nnn = ExtractNNN(opcode);

    vm.Pc = nnn + vm.Vx[0];

    if (m_PrintInstruction)
        std::cout << "Set program counter to " << vm.Pc << std::endl;
}



#pragma region Helper functions to extract values from opcode

int CHIP::OpCodeHandler::ExtractX(uint16_t opcode)
{
    return ((opcode & 0x0F00) >> 8);
}

uint16_t CHIP::OpCodeHandler::ExtractNNN(uint16_t opcode)
{
    return (opcode & 0x0FFF);
}

uint8_t CHIP::OpCodeHandler::ExtractNN(uint16_t opcode)
{
    return (opcode & 0x00FF);
}

int CHIP::OpCodeHandler::ExtractY(uint16_t opcode)
{
    return ((opcode & 0x00F0) >> 4);
}
#pragma endregion
