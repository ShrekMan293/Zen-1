#include "cpu.h"

cpu_controller cpu;

void initCPU() {
	cpu.cores = new CPU_State[8];
	cpu.cpu_threads = new std::thread[8];
}