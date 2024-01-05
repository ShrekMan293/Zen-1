#include "cpu.h"

cpu_controller cpu;

void initCPU() {
	cpu.cores = new CPU_State[8];
	cpu.cpu_threads = new std::thread[8];
	for (size_t i = 0; i < 8; i++) {
		cpu.cores->regs.rx = new uint64_t[28];
		cpu.cores->cpuid = i;
		if (i == 0) {
			cpu.cores->power = AVG;
			continue;
		}
		cpu.cores->power = IDLE;
	}
}