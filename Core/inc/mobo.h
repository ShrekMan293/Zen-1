#pragma once
#include <iostream>
#include <thread>
#include "memory.h"

enum pm {	// Power Mode
	DOWN, IDLE, RUNNING
};

enum operation {
	read, write, null
};


struct device {
	pm power;
	word base;		// Address
	word max;		// Address
	byte id;
	bool DMA;
};

struct mobo_t {
	device info;
	std::thread thread;
};

struct scu_t {

};

struct storage_t {
	uint64_t pollLocation;
	byte status;
	std::string path;	// Virtual disk path
	bool is_on;
};

struct motherboardRequest {
	byte core;
	word gate;
	uint64_t data;
	operation op;
};

struct interruptRequest {
	word gate;
	byte value;
	size_t* registers;
	uint64_t* data;
};

extern motherboardRequest request;
const motherboardRequest nullMBRequest = { 0, 0, null };

extern interruptRequest irq;
const interruptRequest nullIRQ = { 0, 0, nullptr, nullptr };

extern mobo_t controller;

extern mobo_t storage;
extern storage_t storageDevice;

extern mobo_t audio;
extern mobo_t graphics;
extern mobo_t timer;
extern mobo_t rtc;
extern mobo_t usb;
extern mobo_t rng;
//extern mobo_t network;
extern mobo_t scu;

void addIORequest8(word gate, byte data);
void addIORequest16(word gate, word data);
void addIORequest32(word gate, uint32_t data);
void addIORequest64(word gate, uint64_t data);

void createIRQ(word gate, byte value, size_t* registers, uint64_t* data, byte registerCount);
void initComponents();


/*
* Devices:
*	Motherboard Controller (No Gate)
*	Storage (0x4D00 - 0x4D09)
*	Audio (0x3CE0 - 0x3CE8)
*	Graphics (0xD200 - 0xD204)
*	Timer (0xC000 - 0xC00A)
*	RTC (Real Time Controller) (0x11A0 - 0x11A9)
*	USB Controller (0xB800 - 0xB80A)
*	Random Number Gen (0x9000)
*	Network (Will not implement yet)
*	SCU (System Control Unit) (0x0000 - 0x0D08)
*/