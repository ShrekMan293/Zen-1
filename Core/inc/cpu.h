#pragma once
#include "memory.h"
#include "mobo.h"
#include <thread>

using std::thread;

struct registers {
	uint64_t rx[28];		// 0-27
	uint64_t rsp;
	uint64_t rbp;
	byte rf0;
	uint64_t rf1;
	uint64_t rf2;
	byte rf3;
	uint64_t rpp;	// Register Program Pointer
};

struct CPU_State {
	pm power;
	registers regs;
	byte cpuid;
	mobo_t component;
	interruptRequest irq_stack[16];
	bool pause;
	bool stepi;
};


struct cpu_controller {
	CPU_State cores[8];
	thread* cpu_threads;
};

extern cpu_controller cpu;

void initCPU();
void reInitCPU(byte core);