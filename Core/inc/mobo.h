#pragma once
#include <iostream>
#include <thread>
#include "memory.h"

enum pm {	// Power Mode
	DOWN, IDLE, RUNNING
};


struct device {
	pm power;
	word base;		// Address
	word max;		// Address
	bool DMA;
};

struct mobo_t {
	device info;
	std::thread thread;
};

extern mobo_t controller;
extern mobo_t storage;
extern mobo_t audio;
extern mobo_t graphics;
extern mobo_t timer;
extern mobo_t rtc;
extern mobo_t usb;
extern mobo_t rng;
//extern mobo_t network;
extern mobo_t scu;

void initComponents();


/*
* Devices:
*	Motherboard Controller (No Gate)
*	Storage (0x4D00 - 0x4D10)
*	Audio (0x3CE0 - 0x3CE8)
*	Graphics (0xD200 - 0xD209)
*	Timer (0xC000 - 0xC00A)
*	RTC (Real Time Controller) (0x11A0 - 0x11A9)
*	USB Controller (0xB800 - 0xB80A)
*	Random Number Gen (0x9000)
*	Network (Will not implement yet)
*	SCU (System Control Unit)
*/