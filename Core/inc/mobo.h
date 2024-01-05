#pragma once
#include <iostream>
#include <thread>

enum pm {	// Power Mode
	IDLE, LOW,
	AVG, HIGH
};

struct device {
	pm power;
	uint16_t base;		// Address
	uint16_t max;		// Address
	bool DMA;
};

struct mobo_t {
	device* devices;
	std::thread* device_threads;
};



void initComponents();


/*
* Devices:
*	Motherboard Controller
*	Storage
*	Audio
*	Graphics
*	Timer
*	RTC
*	USB Controller
*	Bluetooth Controller
*	Random Number Gen
*	Network (Will not implement yet)
*	SCU
*/