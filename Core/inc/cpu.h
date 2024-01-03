#pragma once
#include "mobo.h"
#include <iostream>

struct cpu_regs {
	uint64_t* rx;
	uint64_t* rf;
	uint64_t rpp;
};

LPTHREAD_START_ROUTINE createCPU(uint8_t index, uint64_t rpp, cpu_regs* cpu);

extern cpu_regs cpu0;
LPTHREAD_START_ROUTINE core0 = createCPU(0, 0x7E00, &cpu0);
extern cpu_regs cpu1;
LPTHREAD_START_ROUTINE core1 = createCPU(1, 0x0000, &cpu1);
extern cpu_regs cpu2;
LPTHREAD_START_ROUTINE core2 = createCPU(2, 0x0000, &cpu2);
extern cpu_regs cpu3;
LPTHREAD_START_ROUTINE core3 = createCPU(3, 0x0000, &cpu3);
extern cpu_regs cpu4;
LPTHREAD_START_ROUTINE core4 = createCPU(4, 0x0000, &cpu4);
extern cpu_regs cpu5;
LPTHREAD_START_ROUTINE core5 = createCPU(5, 0x0000, &cpu5);
extern cpu_regs cpu6;
LPTHREAD_START_ROUTINE core6 = createCPU(6, 0x0000, &cpu6);
extern cpu_regs cpu7;
LPTHREAD_START_ROUTINE core7 = createCPU(7, 0x0000, &cpu7);