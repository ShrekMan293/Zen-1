#pragma once
#include "mobo.h"

struct registers {
	uint64_t* rx;		// 0-27
	uint64_t rsp;
	uint64_t rbp;
	uint8_t rf0;
	uint64_t rf1;
	uint64_t rf2;
	uint64_t rf3;
	uint64_t rf4;
	uint8_t rf5;
	uint8_t pdr;
	uint64_t rpp;	// Register Program Pointer
};

struct CPU_State {
	pm power;
	registers regs;
	uint8_t cpuid;
};

struct cpu_controller {
	CPU_State* cores;
	std::thread* cpu_threads;
};

void initCPU();

// Instructions

// Math
constexpr auto ADD64 = 0b111110000000;
constexpr auto ADD32 = 0b011110000000;
constexpr auto SUB64 = 0b111110000001;
constexpr auto SUB32 = 0b011110000001;
constexpr auto MUL64 = 0b111110000010;
constexpr auto MUL32 = 0b011110000010;
constexpr auto DIV64 = 0b111110000011;
constexpr auto DIV32 = 0b011110000011;

// Logic
constexpr auto REG_SHL64 = 0b111111000100;
constexpr auto IMM_SHL64 = 0b111111010100;
constexpr auto REG_SHL32 = 0b011111000100;
constexpr auto IMM_SHL32 = 0b011111010100;
constexpr auto REG_SHR64 = 0b111111000101;
constexpr auto IMM_SHR64 = 0b111111010101;
constexpr auto REG_SHR32 = 0b011111000101;
constexpr auto IMM_SHR32 = 0b011111010101;
constexpr auto AND64 = 0b111111000110;
constexpr auto AND32 = 0b011111000110;
constexpr auto OR64 = 0b111111000111;
constexpr auto OR32 = 0b011111000111;
constexpr auto NOT64 = 0b111111001000;
constexpr auto NOT32 = 0b011111001000;
constexpr auto NOR64 = 0b111111001001;
constexpr auto NOR32 = 0b011111001001;
constexpr auto XOR64 = 0b111111001010;
constexpr auto XOR32 = 0b011111001010;
constexpr auto CMP64 = 0b111111001011;
constexpr auto CMP32 = 0b011111001011;

// Memory
/* ITI is immediate and immediate (0b00), RTR is register and register (0b11),
RTI is register and immediate (0b10), ITR is immediate and register (0b01) */
constexpr auto ITI_MOV64 = 0b00101111010000;
constexpr auto ITI_MOV32 = 0b00001111010000;
constexpr auto RTR_MOV64 = 0b11101111010000;
constexpr auto RTR_MOV32 = 0b11001111010000;
constexpr auto RTI_MOV64 = 0b10101111010000;
constexpr auto RTI_MOV32 = 0b10001111010000;
constexpr auto ITR_MOV64 = 0b01101111010000;
constexpr auto ITR_MOV32 = 0b01001111010000;
constexpr auto RTI_JMP64 = 0b10100101010001;	// It is RTI because the immediate is being written to rpp
constexpr auto RTI_JMP32 = 0b10000101010001;
constexpr auto RTR_JMP64 = 0b11100101010001;
constexpr auto RTR_JMP32 = 0b11000101010001;
constexpr auto RTI_CALL64 = 0b10100101010010;
constexpr auto RTI_CALL32 = 0b10000101010010;
constexpr auto RTR_CALL64 = 0b11100101010010;
constexpr auto RTR_CALL32 = 0b11000101010010;
constexpr auto RET64 = 0b00100001000011;
constexpr auto RET32 = 0b00000001000011;
constexpr auto RTI_PUSH64 = 0b10100101010100;
constexpr auto RTI_PUSH32 = 0b10000101010100;
constexpr auto RTR_PUSH64 = 0b11100101010100;
constexpr auto RTR_PUSH32 = 0b11000101010100;
constexpr auto POP64 = 0b11100101010101;
constexpr auto POP32 = 0b11000101010101;
constexpr auto PUSHA64 = 0b00100101000110;
constexpr auto PUSHA32 = 0b00000101000110;
constexpr auto POPA64 = 0b00100101000111;
constexpr auto POPA32 = 0b00000101000111;
constexpr auto POPI64 = 0b11100101011000;
constexpr auto POPI32 = 0b11000101011000;
constexpr auto IRET64 = 0b00100001001001;
constexpr auto IRET32 = 0b00000001001001;

// Conditional