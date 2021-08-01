#include "VM.h"
#include <fstream>
#include <sstream>
#include <vector>

CHIP::VM::VM()
    : m_Memory()
    , Vx()
    , Vi(0)
    , Dt(0)
    , St(0)
    , Pc(0x200) // CHIP programs start at 0x200
    , Sp(0)
    , m_Stack()
    , m_OpCodeHandler()
	, m_DisplayPixelValues(m_DisplayWidth * m_DisplayHeight, 0)
	, m_WaitForInput(false)
	, m_InputRegister(0)
	, m_DisplayUpdated(false)
{
    LoadHexSpritesInMemory();
}

CHIP::VM::~VM()
{

}

void CHIP::VM::LoadHexSpritesInMemory()
{
    // Copied from https://github.com/jamesmcm/chip8go/blob/master/src/vm.go
	
	//0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	//0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
	m_Memory[0x050] = 0xF0;
	m_Memory[0x051] = 0x90;
	m_Memory[0x052] = 0x90;
	m_Memory[0x053] = 0x90;
	m_Memory[0x054] = 0xF0;
	// 1
	m_Memory[0x055] = 0x20;
	m_Memory[0x056] = 0x60;
	m_Memory[0x057] = 0x20;
	m_Memory[0x058] = 0x20;
	m_Memory[0x059] = 0x70;
	// 2
	m_Memory[0x05A] = 0xF0;
	m_Memory[0x05B] = 0x10;
	m_Memory[0x05C] = 0xF0;
	m_Memory[0x05D] = 0x80;
	m_Memory[0x05E] = 0xF0;
	// 3
	m_Memory[0x05F] = 0xF0;
	m_Memory[0x060] = 0x10;
	m_Memory[0x061] = 0xF0;
	m_Memory[0x062] = 0x10;
	m_Memory[0x063] = 0xF0;
	// 4
	m_Memory[0x064] = 0x90;
	m_Memory[0x065] = 0x90;
	m_Memory[0x066] = 0xF0;
	m_Memory[0x067] = 0x10;
	m_Memory[0x068] = 0x10;
	// 5
	m_Memory[0x069] = 0xF0;
	m_Memory[0x06A] = 0x80;
	m_Memory[0x06B] = 0xF0;
	m_Memory[0x06C] = 0x10;
	m_Memory[0x06D] = 0xF0;
	// 6
	m_Memory[0x06E] = 0xF0;
	m_Memory[0x06F] = 0x80;
	m_Memory[0x070] = 0xF0;
	m_Memory[0x071] = 0x90;
	m_Memory[0x072] = 0xF0;
	// 7
	m_Memory[0x073] = 0xF0;
	m_Memory[0x074] = 0x10;
	m_Memory[0x075] = 0x20;
	m_Memory[0x076] = 0x40;
	m_Memory[0x077] = 0x40;
	// 8
	m_Memory[0x078] = 0xF0;
	m_Memory[0x079] = 0x90;
	m_Memory[0x07A] = 0xF0;
	m_Memory[0x07B] = 0x90;
	m_Memory[0x07C] = 0xF0;
	// 9
	m_Memory[0x07D] = 0xF0;
	m_Memory[0x07E] = 0x90;
	m_Memory[0x07F] = 0xF0;
	m_Memory[0x080] = 0x10;
	m_Memory[0x081] = 0xF0;
	// A
	m_Memory[0x082] = 0xF0;
	m_Memory[0x083] = 0x90;
	m_Memory[0x084] = 0xF0;
	m_Memory[0x085] = 0x90;
	m_Memory[0x086] = 0x90;
	// B
	m_Memory[0x087] = 0xE0;
	m_Memory[0x088] = 0x90;
	m_Memory[0x089] = 0xE0;
	m_Memory[0x08A] = 0x90;
	m_Memory[0x08B] = 0xE0;
	// C
	m_Memory[0x08C] = 0xF0;
	m_Memory[0x08D] = 0x80;
	m_Memory[0x08E] = 0x80;
	m_Memory[0x08F] = 0x80;
	m_Memory[0x090] = 0xF0;
	// D
	m_Memory[0x091] = 0xE0;
	m_Memory[0x092] = 0x90;
	m_Memory[0x093] = 0x90;
	m_Memory[0x094] = 0x90;
	m_Memory[0x095] = 0xE0;
	// E
	m_Memory[0x096] = 0xF0;
	m_Memory[0x097] = 0x80;
	m_Memory[0x098] = 0xF0;
	m_Memory[0x099] = 0x80;
	m_Memory[0x09A] = 0xF0;
	// F
	m_Memory[0x09B] = 0xF0;
	m_Memory[0x09C] = 0x80;
	m_Memory[0x09D] = 0xF0;
	m_Memory[0x09E] = 0x80;
	m_Memory[0x09F] = 0x80;

}

void CHIP::VM::LoadRom(const std::string& path)
{
    std::ifstream file{path, std::ios::binary | std::ios::in};

    if(!file)
    {
        std::cerr << "Couldn't open file: " << path << std::endl;
        return;
    }

    uint16_t memoryAddress = 0x200;
	Pc = memoryAddress;
	m_WaitForInput = false;
	ClearDisplay();

    while(file)
    {
        uint8_t byte;
        file.read((char*)&byte, sizeof(byte));
        m_Memory[memoryAddress++] = byte;
    }
}

void CHIP::VM::ReloadRom()
{
	Pc = 0x200;
	m_WaitForInput = false;
	ClearDisplay();
}

void CHIP::VM::PrintDisplay() const
{
	std::stringstream stream;

	auto NrOfPixels = m_DisplayWidth * m_DisplayHeight;
	for (int i = 0; i < NrOfPixels; ++i)
	{
		if (i % m_DisplayWidth == 0)
			stream << "\n";

		char toPrint = m_DisplayPixelValues[i] == 1 ? '#' : ' ';
		stream << toPrint;

	}

	stream << std::endl;

	system("cls");
	std::string test = stream.str();
	std::cout << test;
}

void CHIP::VM::ClearDisplay()
{
	m_DisplayUpdated = true;
	for (auto& value : m_DisplayPixelValues)
        value = 0;
}

const std::vector<uint8_t>& CHIP::VM::GetDisplay() const
{
    return m_DisplayPixelValues;
}

void CHIP::VM::KeyDown(int key)
{
	auto hexKeyValue = ConvertCharToHex(key);

	m_KeyboardState[hexKeyValue] = true;

	// In case the program is waitin for input
	if (m_WaitForInput)
	{
		m_WaitForInput = false;
		Vx[m_InputRegister] = hexKeyValue;
	}
}

void CHIP::VM::KeyUp(int key)
{
	m_KeyboardState[ConvertCharToHex(key)] = false;
}

int CHIP::VM::GetDisplayWidth() const
{
	return m_DisplayWidth;
}

int CHIP::VM::GetDisplayHeight() const
{
	return m_DisplayHeight;
}

bool CHIP::VM::DidDisplayUpdate() const
{
	return m_DisplayUpdated;
}

uint16_t CHIP::VM::ConstructOpCode(uint8_t HighOrder, uint8_t LowOrder) const
{
    uint16_t ho = HighOrder; // 16-bit copy for bit shifting
    uint16_t opcode = (ho << 8) | LowOrder;
    
    return opcode;
}

int CHIP::VM::GetPixelIndex(int x, int y) const
{
	const auto nx = x % m_DisplayWidth;
	const auto ny = y % m_DisplayHeight;

	const auto idx = m_DisplayWidth * ny + nx;

	return idx;
}

int CHIP::VM::ConvertCharToHex(char character) const
{
	// IsDigit's behaviour is undefined if character doesn't fall in this range
	if (character < 0 || character > 0xFF)
		return -1;

	const auto digitDifference = '1' - 0x01;
	const auto charDifference = 'A' - 0x0A;

	return (character - (isdigit(character) ? digitDifference : charDifference));
}

void CHIP::VM::Update()
{
	if (m_WaitForInput)
		return;

    if(Pc >= MemorySize)
    {
        std::cerr << "Program counter overflow" << std::endl;
        return;
    }

	if (Dt > 0)
		--Dt;
	if (St > 0)
		--St;

    // Construct opcode from 2 bytes
	uint8_t HO = m_Memory[Pc];
	uint8_t LO = m_Memory[Pc + 1];
    uint16_t opcode = ConstructOpCode(HO, LO);
    
    Pc += 2;

    m_OpCodeHandler.HandleOpCode(*this, opcode);
}
