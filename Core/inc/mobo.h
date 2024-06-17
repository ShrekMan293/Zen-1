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
	bool enabled;
	std::thread thread;
};

struct scu_t {
	byte command;
	byte workingID;	// ID Being worked with
	byte status;
	uint64_t data;
};

struct storage_t {
	uint64_t pollLocation;
	byte status;
	std::string path;	// Virtual disk path
	bool is_on;
};

struct graphics_t {
	uint64_t framebuffer;
	byte status;
	uint64_t data;
	word width;
	word height;
	byte bpp;
	uint32_t pixelCount;
	bool display;
};

struct timer_t {
	uint64_t value;
	uint64_t originalValue;
	uint64_t frequency;	// In nanoseconds
	byte timerMode;
	std::chrono::steady_clock::time_point lastTick; // In nanoseconds
	bool running;
};

struct motherboardRequest {
	byte core;
	word gate;
	uint64_t data;
	operation op;
};

struct interruptRequest {
	byte core;
	word gate;
	byte value;
	size_t* registers;
	uint64_t* data;
	byte regCount;
};

extern pm systemState;

extern motherboardRequest request;
const motherboardRequest nullMBRequest = { 0, 0, null };

extern interruptRequest irq;
const interruptRequest nullIRQ = { 0, 0, 0, nullptr, nullptr, 0 };

extern mobo_t controller;

extern mobo_t storage;
extern storage_t storageDevice;

extern mobo_t audio;

extern mobo_t graphics;
extern graphics_t graphicDevice;

extern mobo_t timer;
extern timer_t timerDevice;

extern mobo_t rtc;

extern mobo_t usb;

extern mobo_t rng;
//extern mobo_t network;

extern mobo_t scu;
extern scu_t scuDevice;

void addIORequest8(word gate, byte data);
void addIORequest16(word gate, word data);
void addIORequest32(word gate, uint32_t data);
void addIORequest64(word gate, uint64_t data);

void createIRQ(byte core, word gate, byte value, size_t* registers, uint64_t* data, byte registerCount);
void initComponents();


/*
* Devices:
*	Motherboard Controller (No Gate)
*	Storage (0x4D00 - 0x4D09)
*	Audio (0x3CE0 - 0x3CE8)
*	Graphics (0xD200 - 0xD209)
*	Timer (0xC000 - 0xC00A)
*	RTC (Real Time Controller) (0x11A0 - 0x11A9)
*	USB Controller (0xB800 - 0xB802)
*	Random Number Gen (0x9000 - 0x9008)
*	Network (Will not implement yet)
*	SCU (System Control Unit) (0x0000 - 0x0D08)
*/