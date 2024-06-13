#pragma once
#include <thread>

using std::thread;

typedef uint8_t byte;
typedef unsigned short word;

struct memory_struct {
	byte* mem1;
	byte* mem2;
	byte* mem3;
	byte* mem4;
	byte* mem5;
	byte* mem6;
	byte* mem7;
	byte* mem8;
	uint64_t size;
	float sizeGB;
};

enum pm {	// Power Mode
	DOWN, IDLE, RUNNING
};

struct registers {
	uint64_t rx[28];		// 0-27
	uint64_t rsp;
	uint64_t rbp;
	byte rf0;
	uint64_t rf1;
	uint64_t rf2;
	uint64_t rf3;
	uint64_t rf4;
	byte rf5;
	byte pdr;
	uint64_t rpp;	// Register Program Pointer
};

struct CPU_State {
	pm power;
	registers regs;
	byte cpuid;
};


struct cpu_controller {
	CPU_State cores[8];
	thread* cpu_threads;
};

extern cpu_controller cpu;
extern memory_struct memory;	// 8 GB

void initCPU();
void initMem(float memorySize);