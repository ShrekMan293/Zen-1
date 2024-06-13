#include "cpu.h"

cpu_controller cpu = { (pm)0, NULL };

void aluParse(word inst, byte index) {
	uint8_t op1 = 0;
	uint64_t op2 = 0;

	if ((inst & 0b0000010001000000) >= 0b0000010001000000) {	// Logic Operations

	}
	else {	// Math Operations
		inst -= 0x790;
		switch (inst)
		{
		case 0: {

		}
		case 1: {

		}
		case 2: {

		}
		case 3: {

		}
		case 4: {

		}
		case 5: {

		}
		default:	// TODO: Invalid Instruction Interrupt
			break;
		}
	}
}

void internalParse(word inst, byte index) {

}

void ioParse(word inst, byte index) {

}

void runCore(byte index) {
	byte bitMode = 64;

	byte prefetch[512];
	
	fetchMemory(cpu.cores[index].regs.rpp, 512, &prefetch, 0);

	word inst = 0x0000;
	uint64_t op1 = 0;
	uint64_t op2 = 0;

	while (cpu.cores[index].power == RUNNING) {
		fetchMemory(cpu.cores[index].regs.rpp += 2, 2, &inst, 0);
		if ((inst & 0b0000010000000000) >= 0b0000010000000000) {
			aluParse(inst, index);
		}
		else if ((inst & 0b0000000001000000) == 0b000000000001000000) {
			internalParse(inst, index);
		}
		else ioParse(inst, index);
	}
}

static void idle(byte index) {
	while (cpu.cores[index].power == IDLE);
}

static void cpuMain(byte index) {
	while (cpu.cores[index].power != DOWN) {
		if (cpu.cores[index].power == IDLE) {
			idle(index);
		}
		else runCore(index);
	}
}

void initCPU() {
	cpu.cpu_threads = new std::thread[8];

	for (size_t i = 0; i < 8; i++) {
		cpu.cores[i].cpuid = i;
		if (i == 0) cpu.cores[i].power = RUNNING;
		else {
			cpu.cores[i].power = IDLE;
			cpu.cpu_threads[i] = thread(cpuMain, i);
		}

		for (size_t j = 0; j < 28; j++)
		{
			cpu.cores[i].regs.rx[j] = 0;
		}
		cpu.cores[i].regs.rpp = 0x1000;
		cpu.cores[i].regs.rsp = 0x3000;
		cpu.cores[i].regs.rbp = 0x3000;
		cpu.cores[i].regs.rf0 = 0;
		cpu.cores[i].regs.rf1 = 0;
		cpu.cores[i].regs.rf2 = 0;
		cpu.cores[i].regs.rf3 = 0;
		cpu.cores[i].regs.rf4 = 0;
		cpu.cores[i].regs.rf5 = 0;
		cpu.cores[i].regs.pdr = 0;
	}

	cpu.cpu_threads[0] = thread(cpuMain, 0);
}