#include <stdio.h>
#include <stdlib.h>

/* Notes on 6502 (8-bit Processor)
    Program Counter (PC): 16 bits
    Stack Pointer (SP): 
    Stack Registor

*/

// Compiler specific.
using u32 = unsigned int;

// 6502 Specific.
using Byte = unsigned char;  // Going to assume this is a byte.
using Word = unsigned short; // Using this as 2 bytes.

struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    // Zero'ing out memory.
    void Initialize() {
        for ( u32 i = 0; i < MAX_MEM; i++) {
            Data[i] = 0;
        }
    }

    /** Read 1 byte */
    // Overriding [] operator to return data at and address.
    Byte operator[] (u32 Address) const {
        // Assert here Address is < MAX_MEM;
        return Data[Address];
    }
};


struct CPU
{

    Word PC;
    Word SP;

    // Registers
    Byte A; // Accumulator.
    Byte X; // Index Register X.
    Byte Y; // Index Register Y.

    // Processor Status Flags (Bit Field)
    Byte C : 1; // Carry Flag
    Byte Z : 1; // Zero Flag
    Byte I : 1; // IRQ Disable Flag.
    Byte D : 1; // Decimal Mode Flag.
    Byte B : 1; // Break Command Flag.
    Byte V : 1; // Overflow Flag.
    Byte N : 1; // Negative Flag.

    void Reset(Mem& memory) {
        PC = 0xFFFC;
        SP = 0x0100;

        // Clearing all flags for now...
        A = 0; X = 0; Y = 0;
        C = 0; Z = 0; I = 0;
        D = 0; B = 0; V = 0;
        N = 0;

        memory.Initialize();
    }

    // Fetch Byte of data.
    // Each time we fetch we increment the PC to the next instruction.
    Byte FetchByte (u32& Cycles, Mem& memory) {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    // Opcodes
    static constexpr Byte
        INSTRUCTION_LDA_IM = 0xA9;

    void Execute (u32 Cycles, Mem& memory) {
        while (Cycles > 0) {
            Byte Instruction = FetchByte(Cycles, memory);

            switch (Instruction) {
                case INSTRUCTION_LDA_IM: 
                {
                    Byte Value = 
                        FetchByte( Cycles, memory );
                    A = Value;
                    Z = (A == 0);
                    N = (A & 0b10000000) > 0;
                }
                break;
            }
        }
    }
};

int main()
{
    Mem mem;
    CPU cpu;

    cpu.Reset( mem );
    cpu.Execute( 2, mem );

    return 0;
}