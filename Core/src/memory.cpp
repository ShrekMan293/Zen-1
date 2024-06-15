#include "memory.h"

void initMem(float memorySize) {
	memory.mem1 = new byte[(size_t)(134217728 * memorySize)];
	memory.mem2 = new byte[(size_t)(134217728 * memorySize)];
	memory.mem3 = new byte[(size_t)(134217728 * memorySize)];
	memory.mem4 = new byte[(size_t)(134217728 * memorySize)];
	memory.mem5 = new byte[(size_t)(134217728 * memorySize)];
	memory.mem6 = new byte[(size_t)(134217728 * memorySize)];
	memory.mem7 = new byte[(size_t)(134217728 * memorySize)];
	memory.mem8 = new byte[(size_t)(134217728 * memorySize)];

	std::memset(memory.mem1, 0, (size_t)(134217728 * memorySize));
	std::memset(memory.mem2, 0, (size_t)(134217728 * memorySize));
	std::memset(memory.mem3, 0, (size_t)(134217728 * memorySize));
	std::memset(memory.mem4, 0, (size_t)(134217728 * memorySize));
	std::memset(memory.mem5, 0, (size_t)(134217728 * memorySize));
	std::memset(memory.mem6, 0, (size_t)(134217728 * memorySize));
	std::memset(memory.mem7, 0, (size_t)(134217728 * memorySize));
	std::memset(memory.mem8, 0, (size_t)(134217728 * memorySize));

	memory.size = (uint64_t)(1073741824 * memorySize);
	memory.sizeGB = memorySize;
}

memory_struct memory = { 0 };

void writeMemory8(uint64_t memOffset, byte data) {
	if (memOffset < 134217728 * memory.sizeGB) { // Section 1
		memory.mem1[memOffset] = data;
	}
	else if (memOffset >= 134217728 * memory.sizeGB && memOffset < (134217728 * 2) * memory.sizeGB) { // Section 2
		memOffset -= (size_t)(134217728 * memory.sizeGB);
		memory.mem2[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 2) * memory.sizeGB && memOffset < (134217728 * 3) * memory.sizeGB) { // Section 3
		memOffset -= (size_t)((134217728 * 2) * memory.sizeGB);
		memory.mem3[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 3) * memory.sizeGB && memOffset < (134217728 * 4) * memory.sizeGB) { // Section 4
		memOffset -= (size_t)((134217728 * 3) * memory.sizeGB);
		memory.mem4[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 4) * memory.sizeGB && memOffset < (134217728 * 5) * memory.sizeGB) { // Section 5
		memOffset -= (size_t)((134217728 * 4) * memory.sizeGB);
		memory.mem5[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 5) * memory.sizeGB && memOffset < (134217728 * 6) * memory.sizeGB) { // Section 6
		memOffset -= (size_t)((134217728 * 5) * memory.sizeGB);
		memory.mem6[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 6) * memory.sizeGB && memOffset < (134217728 * 7) * memory.sizeGB) { // Section 7
		memOffset -= (size_t)((134217728 * 6) * memory.sizeGB);
		memory.mem7[memOffset] = data;
	}
	else if (memOffset >= (134217728 * 7) * memory.sizeGB && memOffset < (134217728 * 8) * memory.sizeGB) { // Section 8
		memOffset -= (size_t)((134217728 * 7) * memory.sizeGB);
		memory.mem8[memOffset] = data;
	}
	else {
		// TODO: Add Invalid Address Interrupt
	}
}

void writeMemory16(uint64_t memOffset, word data) {
	writeMemory8(memOffset, data & 0xFF);
	writeMemory8(memOffset + 1, (data & 0xFF00) >> 8);
}

void writeMemory32(uint64_t memOffset, uint32_t data) {
	writeMemory16(memOffset, data & 0xFFFF);
	writeMemory16(memOffset + 2, (data & 0xFFFF0000) >> 16);
}

void writeMemory64(uint64_t memOffset, uint64_t data) {
	writeMemory32(memOffset, data & 0xFFFFFFFF);
	writeMemory32(memOffset + 4, (data & 0xFFFFFFFF00000000) >> 32);
}

void fetchMemory(uint64_t memOffset, uint64_t fetchLength, void* destination, uint64_t destinationOffset) {
	byte* byteDestination = (byte*)destination;

	if (memOffset < (size_t)(134217728 * memory.sizeGB)) { // Section 1
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem1[i + memOffset];
		}
	}
	else if (memOffset >= 134217728 * memory.sizeGB && memOffset < (134217728 * 2) * memory.sizeGB) { // Section 2
		memOffset -= (size_t)(134217728 * memory.sizeGB);
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem2[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 2) * memory.sizeGB && memOffset < (134217728 * 3) * memory.sizeGB) { // Section 3
		memOffset -= (size_t)((134217728 * 2) * memory.sizeGB);
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem3[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 3) * memory.sizeGB && memOffset < (134217728 * 4) * memory.sizeGB) { // Section 4
		memOffset -= (size_t)((134217728 * 3) * memory.sizeGB);
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem4[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 4) * memory.sizeGB && memOffset < (134217728 * 5) * memory.sizeGB) { // Section 5
		memOffset -= (size_t)((134217728 * 4) * memory.sizeGB);
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem5[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 5) * memory.sizeGB && memOffset < (134217728 * 6) * memory.sizeGB) { // Section 6
		memOffset -= (size_t)((134217728 * 5) * memory.sizeGB);
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem6[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 6) * memory.sizeGB && memOffset < (134217728 * 7) * memory.sizeGB) { // Section 7
		memOffset -= (size_t)((134217728 * 6) * memory.sizeGB);
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem7[i + memOffset];
		}
	}
	else if (memOffset >= (134217728 * 7) * memory.sizeGB && memOffset < (134217728 * 8) * memory.sizeGB) { // Section 8
		memOffset -= (size_t)((134217728 * 7) * memory.sizeGB);
		for (size_t i = 0; i < fetchLength; i++)
		{
			byteDestination[i + destinationOffset] = memory.mem8[i + memOffset];
		}
	}
	else {
		// TODO: Add Invalid Address Interrupt
	}
}