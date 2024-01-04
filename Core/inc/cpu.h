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
	uint64_t rpp;
};

struct CPU_State {
	pm power;
	registers reg;
};

struct cpu_controller {
	CPU_State* cores;
	std::thread* cpu_threads;
};

void initCPU();

// Instructions