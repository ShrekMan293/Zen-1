#include "cpu.h"

cpu_controller cpu = { (pm)0, NULL };
memory_struct memory = { 0 };

static void writeMemory8(uint64_t memOffset, byte data) {

	if (memOffset < 134217728 * memory.sizeGB) { // Section 1
		memory.mem1[memOffset] = data;
	}
	else if (memOffset >= 134217728 * memory.sizeGB && memOffset < (134217728 * 2) * memory.sizeGB) { // Section 2
		memOffset -= 134217728 * memory.sizeGB;
		memory.mem2[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 2) * memory.sizeGB && memOffset < (134217728 * 3) * memory.sizeGB) { // Section 3
		memOffset -= (134217728 * 2) * memory.sizeGB;
		memory.mem3[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 3) * memory.sizeGB && memOffset < (134217728 * 4) * memory.sizeGB) { // Section 4
		memOffset -= (134217728 * 3) * memory.sizeGB;
		memory.mem4[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 4) * memory.sizeGB && memOffset < (134217728 * 5) * memory.sizeGB) { // Section 5
		memOffset -= (134217728 * 4) * memory.sizeGB;
		memory.mem5[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 5) * memory.sizeGB && memOffset < (134217728 * 6) * memory.sizeGB) { // Section 6
		memOffset -= (134217728 * 5) * memory.sizeGB;
		memory.mem6[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 6) * memory.sizeGB && memOffset < (134217728 * 7) * memory.sizeGB) { // Section 7
		memOffset -= (134217728 * 6) * memory.sizeGB;
		memory.mem7[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 7) * memory.sizeGB && memOffset < (134217728 * 8) * memory.sizeGB) { // Section 8
		memOffset -= (134217728 * 7) * memory.sizeGB;
		memory.mem8[memOffset] = data;
	}
	else {
		// TODO: Add Invalid Address Interrupt
	}
}

static void writeMemory16(uint64_t memOffset, word data) {

	if (memOffset < 134217728 * memory.sizeGB) { // Section 1
		memory.mem1[memOffset] = (data >> 8) & 0xFF;
		memory.mem1[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= 134217728 * memory.sizeGB && memOffset < (134217728 * 2) * memory.sizeGB) { // Section 2
		memOffset -= 134217728 * memory.sizeGB;
		memory.mem2[memOffset] = (data >> 8) & 0xFF;
		memory.mem2[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 2) * memory.sizeGB && memOffset < (134217728 * 3) * memory.sizeGB) { // Section 3
		memOffset -= (134217728 * 2) * memory.sizeGB;
		memory.mem3[memOffset] = (data >> 8) & 0xFF;
		memory.mem3[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 3) * memory.sizeGB && memOffset < (134217728 * 4) * memory.sizeGB) { // Section 4
		memOffset -= (134217728 * 3) * memory.sizeGB;
		memory.mem4[memOffset] = (data >> 8) & 0xFF;
		memory.mem4[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 4) * memory.sizeGB && memOffset < (134217728 * 5) * memory.sizeGB) { // Section 5
		memOffset -= (134217728 * 4) * memory.sizeGB;
		memory.mem5[memOffset] = (data >> 8) & 0xFF;
		memory.mem5[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 5) * memory.sizeGB && memOffset < (134217728 * 6) * memory.sizeGB) { // Section 6
		memOffset -= (134217728 * 5) * memory.sizeGB;
		memory.mem6[memOffset] = (data >> 8) & 0xFF;
		memory.mem6[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 6) * memory.sizeGB && memOffset < (134217728 * 7) * memory.sizeGB) { // Section 7
		memOffset -= (134217728 * 6) * memory.sizeGB;
		memory.mem7[memOffset] = (data >> 8) & 0xFF;
		memory.mem7[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 7) * memory.sizeGB && memOffset < (134217728 * 8) * memory.sizeGB) { // Section 8
		memOffset -= (134217728 * 7) * memory.sizeGB;
		memory.mem8[memOffset] = (data >> 8) & 0xFF;
		memory.mem8[memOffset + 1] = data & 0xFF;
	}
	else {
		// TODO: Add Invalid Address Interrupt
	}
}

static void writeMemory32(uint64_t memOffset, uint32_t data) {

	if (memOffset < 134217728 * memory.sizeGB) { // Section 1
		memory.mem1[memOffset] = (data >> 24) & 0xFF;
		memory.mem1[memOffset + 1] = (data >> 16) & 0xFF;
	}
	else if (memOffset >= 134217728 * memory.sizeGB && memOffset < (134217728 * 2) * memory.sizeGB) { // Section 2
		memOffset -= 134217728 * memory.sizeGB;
		memory.mem2[memOffset] = (data >> 8) & 0xFF;
		memory.mem2[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 2) * memory.sizeGB && memOffset < (134217728 * 3) * memory.sizeGB) { // Section 3
		memOffset -= (134217728 * 2) * memory.sizeGB;
		memory.mem3[memOffset] = (data >> 8) & 0xFF;
		memory.mem3[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 3) * memory.sizeGB && memOffset < (134217728 * 4) * memory.sizeGB) { // Section 4
		memOffset -= (134217728 * 3) * memory.sizeGB;
		memory.mem4[memOffset] = (data >> 8) & 0xFF;
		memory.mem4[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 4) * memory.sizeGB && memOffset < (134217728 * 5) * memory.sizeGB) { // Section 5
		memOffset -= (134217728 * 4) * memory.sizeGB;
		memory.mem5[memOffset] = (data >> 8) & 0xFF;
		memory.mem5[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 5) * memory.sizeGB && memOffset < (134217728 * 6) * memory.sizeGB) { // Section 6
		memOffset -= (134217728 * 5) * memory.sizeGB;
		memory.mem6[memOffset] = (data >> 8) & 0xFF;
		memory.mem6[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 6) * memory.sizeGB && memOffset < (134217728 * 7) * memory.sizeGB) { // Section 7
		memOffset -= (134217728 * 6) * memory.sizeGB;
		memory.mem7[memOffset] = (data >> 8) & 0xFF;
		memory.mem7[memOffset + 1] = data & 0xFF;
	}
	else if (memOffset >= (134217728 * 7) * memory.sizeGB && memOffset < (134217728 * 8) * memory.sizeGB) { // Section 8
		memOffset -= (134217728 * 7) * memory.sizeGB;
		memory.mem8[memOffset] = (data >> 8) & 0xFF;
		memory.mem8[memOffset + 1] = data & 0xFF;
	}
	else {
		// TODO: Add Invalid Address Interrupt
	}
}

static void fetchMemory(uint64_t memOffset, uint64_t fetchLength, void* destination, uint64_t destinationOffset) {
	byte* byteDestination = (byte*)destination;

	if (memOffset < 134217728 * memory.sizeGB) { // Section 1
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem1[i + memOffset];
		}
	}
	else if (memOffset >= 134217728 * memory.sizeGB && memOffset < (134217728 * 2) * memory.sizeGB) { // Section 2
		memOffset -= 134217728 * memory.sizeGB;
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem2[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 2) * memory.sizeGB && memOffset < (134217728 * 3) * memory.sizeGB) { // Section 3
		memOffset -= (134217728 * 2) * memory.sizeGB;
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem3[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 3) * memory.sizeGB && memOffset < (134217728 * 4) * memory.sizeGB) { // Section 4
		memOffset -= (134217728 * 3) * memory.sizeGB;
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem4[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 4) * memory.sizeGB && memOffset < (134217728 * 5) * memory.sizeGB) { // Section 5
		memOffset -= (134217728 * 4) * memory.sizeGB;
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem5[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 5) * memory.sizeGB && memOffset < (134217728 * 6) * memory.sizeGB) { // Section 6
		memOffset -= (134217728 * 5) * memory.sizeGB;
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem6[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 6) * memory.sizeGB && memOffset < (134217728 * 7) * memory.sizeGB) { // Section 7
		memOffset -= (134217728 * 6) * memory.sizeGB;
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem7[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 7) * memory.sizeGB && memOffset < (134217728 * 8) * memory.sizeGB) { // Section 8
		memOffset -= (134217728 * 7) * memory.sizeGB;
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem8[i + memOffset];
		}
	}
	else {
		// TODO: Add Invalid Address Interrupt
	}
}

void runCore(byte index) {
	byte bitMode = 64;

	memory.mem1[0x1000] = 29384;
	
	word inst = 0x0F00;	// Filler
	fetchMemory(cpu.cores[index].regs.rpp, 2, &inst, 0);
	uint64_t op1 = 0;
	uint64_t op2 = 0;
}

static void idle(byte index) {
	while (cpu.cores[index].power == IDLE);
}

void initMem(float memorySize) {
	memory.mem1 = new byte[134217728 * memorySize];
	memory.mem2 = new byte[134217728 * memorySize];
	memory.mem3 = new byte[134217728 * memorySize];
	memory.mem4 = new byte[134217728 * memorySize];
	memory.mem5 = new byte[134217728 * memorySize];
	memory.mem6 = new byte[134217728 * memorySize];
	memory.mem7 = new byte[134217728 * memorySize];
	memory.mem8 = new byte[134217728 * memorySize];

	std::memset(memory.mem1, 0, 134217728 * memorySize);
	std::memset(memory.mem2, 0, 134217728 * memorySize);
	std::memset(memory.mem3, 0, 134217728 * memorySize);
	std::memset(memory.mem4, 0, 134217728 * memorySize);
	std::memset(memory.mem5, 0, 134217728 * memorySize);
	std::memset(memory.mem6, 0, 134217728 * memorySize);
	std::memset(memory.mem7, 0, 134217728 * memorySize);
	std::memset(memory.mem8, 0, 134217728 * memorySize);

	memory.size = 1073741824 * memorySize;
	memory.sizeGB = memorySize;
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