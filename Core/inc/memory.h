#pragma once
#include <iostream>

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

extern memory_struct memory;	// 8 GB
void initMem(float memorySize);

void writeMemory8(uint64_t memOffset, byte data);
void writeMemory16(uint64_t memOffset, word data);
void writeMemory32(uint64_t memOffset, uint32_t data);
void writeMemory64(uint64_t memOffset, uint64_t data);
void fetchMemory(uint64_t memOffset, uint64_t fetchLength, void* destination, uint64_t destinationOffset);