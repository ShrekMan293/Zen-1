#pragma warning(push)
#pragma warning(disable: 6001)
#pragma warning(disable: 26819)

#include "mobo.h"
#include "cpu.h"

#include <fstream>
#include <filesystem>
#include <SDL.h>
#include <random>
#include <ctime>
#include <chrono>

std::uintmax_t fileSize(std::string path) {
	std::filesystem::path filePath = path;
	std::uintmax_t fileSize = std::filesystem::file_size(filePath);
	
	return fileSize;
}

struct usb_t {
	byte mode; // connection and type
	bool enabled;
	bool connected;
};

pm systemState = RUNNING;

motherboardRequest request = { 0 };
interruptRequest irq = { 0 };

mobo_t controller = { (pm)0 };

mobo_t storage = { (pm)0 };
storage_t storageDevice = { 0 };

mobo_t audio = { (pm)0 };

mobo_t graphics = { (pm)0 };
graphics_t graphicDevice = { 0x00000000000C0000, 0x30, 0, 640, 360, 0x00, 640 * 360, true };

mobo_t timer = { (pm)0 };
timer_t timerDevice = { 0 };

mobo_t rtc = { (pm)0 };

mobo_t usb = { (pm)0 };
usb_t usbDevice = { 0, 1, 0 };

mobo_t rng = { (pm)0 };
//mobo_t network = { (pm)0 };

mobo_t scu = { (pm)0 };
scu_t scuDevice = { 0 };

void audioMain() {

}

void storageMain() {
	storageDevice.is_on = 1;
	storageDevice.status = 0x30;

	storage_top:
	while (storage.info.power == RUNNING) {
		uint32_t count = 0;			// Using the count section because it cannot be 0
		uint64_t address = 0;
		uint64_t start = 0;
		uint8_t command = 0;

		while (!storage.enabled);

		fetchMemory(storageDevice.pollLocation + 8, 4, &count, 0);
		if (count != 0) {
			fetchMemory(storageDevice.pollLocation, 8, &address, 0);
			fetchMemory(storageDevice.pollLocation + 12, 8, &start, 0);
			fetchMemory(storageDevice.pollLocation + 20, 1, &command, 0);

			switch (command)
			{
			case 0x00: break;
			case 0x10: storageDevice.status = 0x30; break;
			case 0x30: {
				storageDevice.status = 0x40;
				char* data = new char[count * 512];
				std::ifstream i(storageDevice.path, std::ios::in | std::ios::binary);
				if (!i.is_open()) {
					storageDevice.status = 0x90;
					break;
				}
				if (start >= fileSize(storageDevice.path) / 512 || start + count >= fileSize(storageDevice.path) / 512) {
					storageDevice.status = 0xB0;
					break;
				}

				i.seekg(start * 512);
				if (i.fail()) {
					storageDevice.status = 0xA0;
					break;
				}

				storageDevice.status = 0x10;
				i.read(data, static_cast<std::streamsize>(count) * 512);
				for (size_t i = 0; i < (size_t)count * 512; i++)
				{
					writeMemory8(address + i, data[i]);
				}
				delete[] data;
				if (i.fail()) {
					storageDevice.status = 0xF0;
					break;
				}

				storageDevice.status = 0x30;
				break;
			}
			case 0x40: {
				storageDevice.status = 0x40;
				char* data = new char[(size_t)count * 512];
				std::ofstream o(storageDevice.path, std::ios::out | std::ios::binary);
				if (!o.is_open()) {
					storageDevice.status = 0x90;
					break;
				}
				if (start >= fileSize(storageDevice.path) / 512 || start + count >= fileSize(storageDevice.path) / 512) {
					storageDevice.status = 0xB0;
					break;
				}

				o.seekp(start);
				if (o.fail()) {
					storageDevice.status = 0xA0;
					break;
				}

				fetchMemory(address, (size_t)count * 512, data, 0);
				storageDevice.status = 0x10;
				o.write(data, (size_t)count * 512);
				delete[] data;
				if (o.fail()) {
					storageDevice.status = 0xF0;
					break;
				}

				storageDevice.status = 0x30;
				break;
			}
			case 0xC0: {
				for (size_t j = 0; j < 3; j++)
				{
					storageDevice.status = 0x40;
					char* data = new char[count * 512];
					std::ifstream i(storageDevice.path, std::ios::in | std::ios::binary);
					if (!i.is_open()) {
						storageDevice.status = 0x90;
						break;
					}
					if (start >= fileSize(storageDevice.path) / 512 || start + count >= fileSize(storageDevice.path) / 512) {
						storageDevice.status = 0xB0;
						break;
					}

					i.seekg(start * 512);
					if (i.fail()) {
						storageDevice.status = 0xA0;
						break;
					}

					storageDevice.status = 0x10;
					i.read(data, static_cast<std::streamsize>(count) * 512);
					for (size_t i = 0; i < (size_t)count * 512; i++)
					{
						writeMemory8(address + i, data[i]);
					}
					delete[] data;
					if (i.fail()) {
						storageDevice.status = 0xF0;
						break;
					}

					storageDevice.status = 0x30;
				}
				break;
			}
			case 0xD0: {
				for (size_t j = 0; j < 3; j++)
				{
					storageDevice.status = 0x40;
					char* data = new char[(size_t)count * 512];
					std::ofstream o(storageDevice.path, std::ios::out | std::ios::binary);
					if (!o.is_open()) {
						storageDevice.status = 0x90;
						break;
					}
					if (start >= fileSize(storageDevice.path) / 512 || start + count >= fileSize(storageDevice.path) / 512) {
						storageDevice.status = 0xB0;
						break;
					}

					o.seekp(start);
					if (o.fail()) {
						storageDevice.status = 0xA0;
						break;
					}

					fetchMemory(address, (size_t)count * 512, data, 0);
					storageDevice.status = 0x10;
					o.write(data, (size_t)count * 512);
					delete[] data;
					if (o.fail()) {
						storageDevice.status = 0xF0;
						break;
					}

					storageDevice.status = 0x30;
				}
				break;
			}
			case 0xEC: {
				uintmax_t size = fileSize(storageDevice.path) / 512;
				writeMemory64(address, size);
				break;
			}
			case 0xFA: {
				storageDevice.status = 0x10;
				std::vector<char> zeros(512, 0);
				std::fstream disk(storageDevice.path, std::ios::out | std::ios::binary);

				if (!disk.is_open()) {
					storageDevice.status = 0x90;
					break;
				}

				std::uintmax_t diskSize = fileSize(storageDevice.path);
				uint64_t numSectors = diskSize / 512;

				for (size_t i = 0; i < numSectors; i++)
				{
					disk.seekp(i * 512);
					if (disk.fail()) {
						storageDevice.status = 0xA0;
						break;
					}
					disk.write(zeros.data(), zeros.size());
					if (disk.fail()) {
						storageDevice.status = 0xF0;
						break;
					}
				}

				if (storageDevice.status == 0x10) storageDevice.status = 0x30;
				break;
			}
			case 0xFF: {
				if (storageDevice.is_on) storageDevice.is_on = 0;
				else storageDevice.is_on = 1;
				break;
			}
			default:
				storageDevice.status = 0x80;
				break;
			}
		}
		if (storageDevice.status != 0x30) continue;

		storageDevice.status = 0x30; // Ready

		if (request.gate == 0x4D00) {
			if (request.op == read) cpu.cores[request.core].regs.rx[request.data] = storageDevice.pollLocation;
			else storageDevice.pollLocation = request.data;
		}
		else if (request.gate == 0x4D08) {
			if (request.op != read) {
				createIRQ(0, (word)request.data, 0x08, nullptr, nullptr, 0);
			}
			else cpu.cores[request.core].regs.rx[request.data] = storageDevice.status;
		}

		request = nullMBRequest;
	}
	if (storage.info.power == IDLE || !storageDevice.is_on) {
		while (storage.info.power == IDLE || !storageDevice.is_on);
		goto storage_top;	// I know. I'm sorry
	}
	else storageDevice.status = 0xD0;
}

void graphicsMain() {
	SDL_Window* window = SDL_CreateWindow("Zen-1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	Uint32 pixelFormat = SDL_PIXELFORMAT_RGB332;
	SDL_Texture* screen = SDL_CreateTexture(renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING, 640, 360);

	// framebuffer_(BPP)
	byte* framebuffer = new byte[640 * 360];


	if (window == nullptr || renderer == nullptr) {
		do {
			request.core = 0;
			request.data = 0xB0;
			request.gate = 0x0000;
			request.op = write;

			std::this_thread::sleep_for(std::chrono::microseconds(10));

			request.core = 0;
			request.data = 27;
			request.gate = 0x0000;
			request.op = read;
		} while (cpu.cores[0].regs.rx[27] != 0xB0);

		request.core = 0;
		request.data = 0xFF;
		request.gate = 0x00C0;
		request.op = write;
	}

	graphics_top:
	while (graphics.info.power == RUNNING && graphics.enabled) {
		switch (request.gate) {
		case 0xD200: {
			if (request.op == read) break;

			switch (request.data) {
			case 0x10: graphicDevice.data = graphicDevice.framebuffer; break;
			case 0x12: graphicDevice.framebuffer = graphicDevice.data; break;
			case 0x14: graphicDevice.data = graphicDevice.width; break;
			case 0x16: {
				graphicDevice.width = (word)graphicDevice.data;
				SDL_DestroyTexture(screen);
				screen = SDL_CreateTexture(renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING,
					graphicDevice.width, graphicDevice.height);

				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, screen, NULL, NULL);
				SDL_RenderPresent(renderer);
				break;
			}
			case 0x18: graphicDevice.data = graphicDevice.height; break;
			case 0x1A: {
				graphicDevice.height = (word)graphicDevice.data;
				SDL_DestroyTexture(screen);
				screen = SDL_CreateTexture(renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING,
					graphicDevice.width, graphicDevice.height);

				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, screen, NULL, NULL);
				SDL_RenderPresent(renderer);
				break;
			}
			case 0x1C: {
				if (graphicDevice.display) {
					SDL_RenderClear(renderer);
					SDL_RenderPresent(renderer);
				}
				else {
					SDL_RenderPresent(renderer);
				}
				break;
			}
			case 0x2A: {
				switch (request.data) {
				case 0x00: pixelFormat = SDL_PIXELFORMAT_RGB332; graphicDevice.bpp = 0x00; break;
				case 0x01: pixelFormat = SDL_PIXELFORMAT_ARGB4444; graphicDevice.bpp = 0x01; break;
				case 0x02: pixelFormat = SDL_PIXELFORMAT_ARGB1555; graphicDevice.bpp = 0x02; break;
				case 0x03: pixelFormat = SDL_PIXELFORMAT_RGB888; graphicDevice.bpp = 0x03; break;
				case 0x04: pixelFormat = SDL_PIXELFORMAT_ARGB8888; graphicDevice.bpp = 0x04; break;
				default: graphicDevice.status = 0xA0; break;
				}
				break;
			}
			case 0x2C: graphicDevice.data = graphicDevice.bpp; break;
			case 0xFF: graphicDevice.data = graphicDevice.status; break;
			default: graphicDevice.status = 0x80; break;
			}
			break;
		}
		case 0xD201: {
			if (request.op == read) {
				cpu.cores[request.core].regs.rx[request.data] = graphicDevice.data;
			}
			else {
				graphicDevice.data = request.data;
			}
			break;
		}
		default: break;
		}

		request = nullMBRequest;

		fetchMemory(graphicDevice.framebuffer, graphicDevice.pixelCount * static_cast<uint64_t>(graphicDevice.bpp / 8),
			framebuffer, 0);
		SDL_UpdateTexture(screen, NULL, framebuffer, graphicDevice.width * (graphicDevice.bpp / 8));

		if (!graphicDevice.display) continue;

		if (SDL_RenderClear(renderer) < 0) graphicDevice.status = 0xDF;
		if (SDL_RenderCopy(renderer, screen, NULL, NULL) < 0) graphicDevice.status = 0xDF;
		SDL_RenderPresent(renderer);
	}

	while (graphics.info.power == IDLE);
	while (!graphics.enabled);

	if (graphics.info.power == DOWN) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyTexture(screen);
		SDL_DestroyWindow(window);
		return;
	}

	goto graphics_top;	// Sorry again
}

void timerMain() {
	byte mode = 0;

	timer_top:
	while (timer.info.power == RUNNING && timer.enabled) {
		if (timerDevice.running) {
			switch (timerDevice.timerMode) {
			case 0b00: {
				auto currentTick = std::chrono::high_resolution_clock::now();
				if ((static_cast<std::chrono::nanoseconds>(currentTick - timerDevice.lastTick).count() / timerDevice.frequency)
					> timerDevice.value) {
					timerDevice.value = 0;
					timerDevice.running = 0;
					createIRQ(0, timer.info.base, 0x09, nullptr, nullptr, 0);
				}
				else timerDevice.value -= (static_cast<std::chrono::nanoseconds>(currentTick - timerDevice.lastTick).count() / timerDevice.frequency);
				break;
			}
			case 0b01: {
				auto currentTick = std::chrono::high_resolution_clock::now();
				if ((static_cast<std::chrono::nanoseconds>(currentTick - timerDevice.lastTick).count() / timerDevice.frequency)
					> timerDevice.value) {
					timerDevice.value = 0;
					timerDevice.running = 1;
					createIRQ(0, timer.info.base, 0x09, nullptr, nullptr, 0);
				}
				else timerDevice.value -= (static_cast<std::chrono::nanoseconds>(currentTick - timerDevice.lastTick).count() / timerDevice.frequency);
				break;
			}
			case 0b10: {
				auto currentTick = std::chrono::high_resolution_clock::now();
				if ((static_cast<std::chrono::nanoseconds>(currentTick - timerDevice.lastTick).count() / timerDevice.frequency)
					> timerDevice.value) {
					timerDevice.value = 0;
					timerDevice.running = 0;
					createIRQ(0, timer.info.base, 0x09, nullptr, nullptr, 0);
				}
				else timerDevice.value -= (static_cast<std::chrono::nanoseconds>(currentTick - timerDevice.lastTick).count() / timerDevice.frequency);
				break;
			}
			case 0b11: {
				auto currentTick = std::chrono::high_resolution_clock::now();
				if ((static_cast<std::chrono::nanoseconds>(currentTick - timerDevice.lastTick).count() / timerDevice.frequency)
					> timerDevice.value) {
					timerDevice.value = 0;
					timerDevice.running = 1;
					createIRQ(0, timer.info.base, 0x09, nullptr, nullptr, 0);
				}
				else timerDevice.value -= (static_cast<std::chrono::nanoseconds>(currentTick - timerDevice.lastTick).count() / timerDevice.frequency);
				break;
			}
			}
		}

		switch (request.gate) {
		case 0xC002: {
			if (request.op == read) break;
			
			mode = (byte)request.data;

			switch (mode & 0xF) {
			case 0b00: timerDevice.frequency = 1000000000; break;	// 1 tick per 1 billion nanoseconds (1 Hz)
			case 0b01: timerDevice.frequency = 1000000; break;	// 1 tick per 1 million nanoseconds (1000 Hz)
			case 0b10: timerDevice.frequency = 1000; break;		// 1 tick per 1 thousand nanoseconds (1 MHz)
			case 0b11: timerDevice.frequency = 1; break;			// 1 tick per nanosecond (1 GHz)
			default: {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = request.data;
				createIRQ(0, timer.info.base, 0x0A, registers, data, 0);
				break;
			}
			}

			timerDevice.timerMode = (mode & 0xF0) >> 4;

			break;
		}
		case 0xC009: {
			if (request.op == read) cpu.cores[request.core].regs.rx[request.data] = timerDevice.value;
			else {
				timerDevice.value = request.data;
				timerDevice.running = true;
			}
			break;
		}
		default: break;
		}

		request = nullMBRequest;
	}

	while (timer.info.power == IDLE);
	while (!timer.enabled);

	if (timer.info.power == DOWN) return;

	goto timer_top;	// Once again, sorry
}

void usbMain() {
	int numControllers = SDL_NumJoysticks();
	if (numControllers < 1) {
		usbDevice.enabled = false;
		usbDevice.connected = false;
	}
	else usbDevice.connected = true;

	SDL_GameController* ps = SDL_GameControllerOpen(0);
	if (ps == nullptr) usbDevice.mode = 0x03;
	SDL_Event e;

	size_t* registers = nullptr;
	uint64_t* data = nullptr;

	usb_top:
	while (usb.info.power == RUNNING && usb.enabled) {
		switch (request.gate) {
		case 0xB800: usbDevice.enabled = !usbDevice.enabled; break;
		case 0xB801: {
			if (request.op == read) cpu.cores[request.core].regs.rx[request.data] = usbDevice.mode;
			else return;
			break;
		}
		default: break;
		}

		if (usbDevice.enabled) {
			SDL_PollEvent(&e);
			switch (e.type) {
			case SDL_QUIT: systemState = DOWN; break;
			case SDL_CONTROLLERBUTTONDOWN: {
				registers = new size_t[1];
				registers[0] = 4;
				data = new uint64_t[1];
				data[0] = e.cbutton.button;
				createIRQ(0, 0xB800, 0x10, registers, data, 1);
				break;
			}
			case SDL_CONTROLLERBUTTONUP: {
				registers = new size_t[1];
				registers[0] = 4;
				data = new uint64_t[1];
				data[0] = e.cbutton.button;
				createIRQ(0, 0xB800, 0x11, registers, data, 1);
				break;
			}
			case SDL_CONTROLLERAXISMOTION: {
				registers = new size_t[2];
				registers[0] = 4;
				registers[1] = 5;
				data = new uint64_t[2];
				switch (e.caxis.axis)
				{
				case SDL_CONTROLLER_AXIS_LEFTX: {
					data[0] = 0;
					data[1] = e.caxis.value;
					break;
				}
				case SDL_CONTROLLER_AXIS_LEFTY: {
					data[0] = 1;
					data[1] = e.caxis.value;
					break;
				}
				case SDL_CONTROLLER_AXIS_RIGHTX: {
					data[0] = 2;
					data[1] = e.caxis.value;
					break;
				}
				case SDL_CONTROLLER_AXIS_RIGHTY: {
					data[0] = 3;
					data[1] = e.caxis.value;
					break;
				}
				case SDL_CONTROLLER_AXIS_TRIGGERLEFT: {
					data[0] = 4;
					data[1] = e.caxis.value;
					break;
				}
				case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: {
					data[0] = 5;
					data[1] = e.caxis.value;
					break;
				}
				default:
					break;
				}
				break;
			}
			case SDL_CONTROLLERDEVICEREMOVED: {
				numControllers--;
				if (numControllers == 0) {
					usbDevice.connected = false;
					usbDevice.mode = 2;
					ps = nullptr;
					break;
				}
			}
			case SDL_CONTROLLERDEVICEADDED: {
				if (numControllers == 0) {
					numControllers++;
					usbDevice.connected = true;
					usbDevice.mode = 1;
					ps = SDL_GameControllerOpen(0);
					if (ps == nullptr) {
						usbDevice.connected = false;
						usbDevice.mode = 3;
					}
					break;
				}
				else break;
			}
			}
		}
	}

	if (usb.info.power == DOWN) {
		SDL_GameControllerClose(ps);
		return;
	}

	while (usb.info.power == IDLE);
	while (!usb.enabled);

	goto usb_top;
}

mobo_t* getDeviceFromID(byte id) {
	switch (id) {
	case 0x00: return &controller;
	case 0x01: return &scu;
	case 0x10: return &cpu.cores[0].component;
	case 0x11: return &cpu.cores[1].component;
	case 0x12: return &cpu.cores[2].component;
	case 0x13: return &cpu.cores[3].component;
	case 0x14: return &cpu.cores[4].component;
	case 0x15: return &cpu.cores[5].component;
	case 0x16: return &cpu.cores[6].component;
	case 0x17: return &cpu.cores[7].component;
	case 0x20: return &timer;
	case 0x21: return &rtc;
	case 0x60: return &usb;
	case 0x80: return &storage;
	case 0xA0: return &rng;
	case 0xB0: return &graphics;
	case 0xE0: return &audio;
	default: {
		size_t* registers = new size_t[1];
		uint64_t* data = new uint64_t[1];
		registers[0] = 4;
		data[0] = request.data;
		createIRQ(0, timer.info.base, 0x0B, registers, data, 0);
		return nullptr;
	}
	}
}

void onPollMain(mobo_t* device) {
	switch (device->info.base)
	{
	case 0x11A0: {	// RTC
		if (rtc.info.power != RUNNING);
		else {
			if (request.op == write) break;
			else request.data = std::time(nullptr);
		}
		break;
	}
	case 0x9000: {	// RNG
		if (rng.info.power != RUNNING);
		else {
			std::random_device rd;
			std::mt19937_64 eng(rd());
			std::uniform_int_distribution<uint64_t> gen;

			if (request.op == write) break;
			else request.data = gen(eng);

			request = nullMBRequest;
		}

		break;
	}
	case 0x0000: {	// SCU
		switch (request.gate)
		{
		case 0x0000: {
			if (request.op == read) cpu.cores[request.core].regs.rx[request.data] = scuDevice.workingID;
			else scuDevice.workingID = (byte)request.data;

			request = nullMBRequest;
			break;
		}
		case 0x00C0: {
			if (request.op == read) return;
			else scuDevice.command = (byte)request.data;

			switch (scuDevice.command) {
			case 0x10: {
				if (scuDevice.workingID >= 0x10 && scuDevice.workingID <= 0x17) {
					cpu.cores[scuDevice.workingID - 0x10].power = (pm)scuDevice.data;
				}
				getDeviceFromID(scuDevice.workingID)->info.power = (pm)scuDevice.data;
				break;
			}
			case 0x20: scuDevice.data = getDeviceFromID(scuDevice.workingID)->info.power; break;
			case 0x30: {
				switch (scuDevice.workingID) {
				case 0x00: break;	// Can't reset
				case 0x01: break;	// Can't reset
				case 0x10: {
					reInitCPU(0);
					break;
				}
				case 0x11: {
					reInitCPU(1);
					break;
				}
				case 0x12: {
					reInitCPU(2);
					break;
				}
				case 0x13: {
					reInitCPU(3);
					break;
				}
				case 0x14: {
					reInitCPU(4);
					break;
				}
				case 0x15: {
					reInitCPU(5);
					break;
				}
				case 0x16: {
					reInitCPU(6);
					break;
				}
				case 0x17: {
					reInitCPU(7);
					break;
				}
				case 0x20: {
					timerDevice.frequency = 0;
					timerDevice.originalValue = 0;
					timerDevice.running = 0;
					timerDevice.timerMode = 0;
					timerDevice.value = 0;

					timer.info.power = IDLE;
					timer.info.base = 0xC000;
					timer.info.max = 0xC009;
					timer.info.DMA = 0;
					timer.info.id = 0x20;
					timer.enabled = true;
					break;
				}
				case 0x21: {
					rtc.info.power = RUNNING;
					rtc.info.base = 0x11A0;
					rtc.info.max = 0x11A7;
					rtc.info.DMA = 0;
					rtc.info.id = 0x21;
					rtc.enabled = true;
					break;
				}
				case 0x60: {
					break; // TODO: USB
				}
				case 0x80: {
					storage.info.power = IDLE;
					storage.info.base = 0x4D00;
					storage.info.max = 0x4D09;
					storage.info.DMA = 1;
					storage.info.id = 0x80;

					storageDevice.is_on = 1;
					storageDevice.pollLocation = 0;
					storageDevice.status = 0x30;
					break;
				}
				case 0xA0: break; // Allowed to reset, but can't really do anything
				case 0xB0: {
					graphicDevice.bpp = 0x00;
					graphicDevice.data = 0;
					graphicDevice.display = 1;
					graphicDevice.framebuffer = 0xC0000;
					graphicDevice.height = 360;
					graphicDevice.width = 640;
					graphicDevice.status = 0x30;
					graphicDevice.pixelCount = 360 * 640;

					graphics.info.power = RUNNING;
					graphics.info.base = 0xD200;
					graphics.info.max = 0xD209;
					graphics.info.DMA = 1;
					graphics.info.id = 0xB0;
					graphics.enabled = true;
					break;
				}
				case 0xE0: {
					break; // TODO: Audio
				}
				default: {
					size_t* registers = new size_t[1];
					uint64_t* data = new uint64_t[1];
					registers[0] = 4;
					data[0] = request.data;
					createIRQ(0, timer.info.base, 0x0B, registers, data, 0);
					break;
				}
				}
				break;
			}
			case 0x40: {
				getDeviceFromID(scuDevice.workingID)->enabled = true;
				break;
			}
			case 0x50: {
				getDeviceFromID(scuDevice.workingID)->enabled = false;
				break;
			}
			case 0x60: {
				/* IDs are given in this order:
				* SCU (Highest Bit)
				* Storage
				* Graphics
				* Audio
				* USB
				* RTC
				* Timer
				* RNG (Lowest Bit)
				*/

				scuDevice.data = 0x0180B0E0602120A0;
				break;
			}
			case 0x80: {
				scuDevice.data = scuDevice.status;
				break;
			}
			case 0xF0: {
				storage.info.power = DOWN;
				audio.info.power = DOWN;
				timer.info.power = DOWN;
				rtc.info.power = DOWN;
				rng.info.power = DOWN;
				usb.info.power = DOWN;
				cpu.cores[7].power = DOWN;
				cpu.cores[7].component.info.power = DOWN;
				cpu.cores[6].power = DOWN;
				cpu.cores[6].component.info.power = DOWN;
				cpu.cores[5].power = DOWN;
				cpu.cores[5].component.info.power = DOWN;
				cpu.cores[4].power = DOWN;
				cpu.cores[4].component.info.power = DOWN;
				cpu.cores[3].power = DOWN;
				cpu.cores[3].component.info.power = DOWN;
				cpu.cores[2].power = DOWN;
				cpu.cores[2].component.info.power = DOWN;
				cpu.cores[1].power = DOWN;
				cpu.cores[1].component.info.power = DOWN;
				graphics.info.power = DOWN;
				cpu.cores[0].power = DOWN;
				cpu.cores[0].component.info.power = DOWN;
				scu.info.power = DOWN;
				systemState = DOWN;
				controller.info.power = DOWN;
				break;
			}
			case 0xFF: {
				if (scuDevice.workingID == 0x00 || scuDevice.workingID == 0x01) {
					cpu.cores[0].regs.rf0 |= 0b10;	// Put core in critical mode
					cpu.cores[1].regs.rf0 |= 0b10;
					cpu.cores[2].regs.rf0 |= 0b10;
					cpu.cores[3].regs.rf0 |= 0b10;
					cpu.cores[4].regs.rf0 |= 0b10;
					cpu.cores[5].regs.rf0 |= 0b10;
					cpu.cores[6].regs.rf0 |= 0b10;
					cpu.cores[7].regs.rf0 |= 0b10;
					break;
				}

				getDeviceFromID(scuDevice.workingID)->enabled = false;
				createIRQ(request.core, getDeviceFromID(scuDevice.workingID)->info.base, 0x03, nullptr, nullptr, 0);
				break;
			}
			default:
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = request.data;
				createIRQ(0, timer.info.base, 0x0C, registers, data, 0);
				break;
			}

			request = nullMBRequest;
			break;
		}
		case 0x00D0: {
			if (request.op == read) cpu.cores[request.core].regs.rx[request.data] = scuDevice.data;
			else scuDevice.data = request.data;

			request = nullMBRequest;
			break;
		}
		default:
			request = nullMBRequest;
			break;
		}
		break;
	}
	default:
		break;
	}
}

void motherboardMain() {
	while (controller.info.power == RUNNING) {
		while (request.data == 0 && request.gate == 0
			&& request.core == 0 && request.op == 0
			&& irq.data == nullptr && irq.gate == 0
			&& irq.registers == nullptr && irq.value == 0);

		if (request.data != 0 && request.gate != 0) {
			switch (request.gate)
			{
			case 0x0000: {
				if (!scu.enabled) break;
				onPollMain(&scu);
				break;
			}
			case 0x00C0: {
				if (!scu.enabled) break;
				onPollMain(&scu);
				break;
			}
			case 0x00D0: {
				if (!scu.enabled) break;
				onPollMain(&scu);
				break;
			}
			case 0x9000: {
				if (!rng.enabled) break;
				onPollMain(&rng);
				break;
			}
			default:
				break;
			}

			request = nullMBRequest;
		}
		else continue;
	}
}

void addIORequest8(word gate, byte data)
{
	while (request.data != 0 && request.gate != 0
		&& request.core != 0 && request.op != 0);

	request.gate = gate;
	request.data = (uint64_t)data;
}

void addIORequest16(word gate, word data)
{
	while (request.data != 0 && request.gate != 0
		&& request.core != 0 && request.op != 0);

	request.gate = gate;
	request.data = (uint64_t)data;
}

void addIORequest32(word gate, uint32_t data)
{
	while (request.data != 0 && request.gate != 0
		&& request.core != 0 && request.op != 0);

	request.gate = gate;
	request.data = (uint64_t)data;
}

void addIORequest64(word gate, uint64_t data)
{
	while (request.data != 0 && request.gate != 0
		&& request.core != 0 && request.op != 0);

	request.gate = gate;
	request.data = data;
}

void createIRQ(byte core, word gate, byte value, size_t* registers, uint64_t* data, byte registerCount) {
	if ((cpu.cores[core].regs.rf0 & 0b01000000) == 0) {
		delete[] registers;
		delete[] data;
	}

	irq.core = core;
	irq.gate = gate;
	irq.value = value;
	irq.registers = registers;
	irq.data = data;
	irq.regCount = registerCount;
}

void initComponents() {
	controller.info.power = RUNNING;
	controller.info.base = 0x0000;
	controller.info.max = 0x0000;
	controller.info.DMA = 1;
	controller.info.id = 0x00;
	controller.enabled = true;
	controller.thread = std::thread(motherboardMain);

	storage.info.power = IDLE;
	storage.info.base = 0x4D00;
	storage.info.max = 0x4D08;
	storage.info.DMA = 1;
	storage.info.id = 0x80;
	storage.enabled = true;
	storage.thread = std::thread(storageMain);

	audio.info.power = IDLE;
	audio.info.base = 0x3CE0;
	audio.info.max = 0x3CE7;
	audio.info.DMA = 1;
	audio.info.id = 0xE0;
	audio.enabled = true;
	audio.thread = std::thread(audioMain);

	graphics.info.power = RUNNING;
	graphics.info.base = 0xD200;
	graphics.info.max = 0xD208;
	graphics.info.DMA = 1;
	graphics.info.id = 0xB0;
	graphics.enabled = true;
	graphics.thread = std::thread(graphicsMain);

	timer.info.power = IDLE;
	timer.info.base = 0xC000;
	timer.info.max = 0xC009;
	timer.info.DMA = 0;
	timer.info.id = 0x20;
	timer.enabled = true;
	timer.thread = std::thread(timerMain);

	rtc.info.power = RUNNING;
	rtc.info.base = 0x11A0;
	rtc.info.max = 0x11A7;
	rtc.info.DMA = 0;
	rtc.info.id = 0x21;
	rtc.enabled = true;
	rtc.thread;	// On Poll

	usb.info.power = IDLE;
	usb.info.base = 0xB800;
	usb.info.max = 0xB802;
	usb.info.DMA = 1;
	usb.info.id = 0x60;
	usb.enabled = true;
	usb.thread = std::thread(usbMain);

	rng.info.power = IDLE;
	rng.info.base = 0x9000;
	rng.info.max = 0x9007;
	rng.info.DMA = 0;
	rng.info.id = 0xA0;
	rng.enabled = true;
	rng.thread;	// On Poll

	scu.info.power = RUNNING;
	scu.info.base = 0x0000;
	scu.info.max = 0x0D07;
	scu.info.DMA = 1;
	scu.info.id = 0x01;
	scu.enabled = true;
	scuDevice.status = 0xFF;	// No point in this command but who cares
	scu.thread;	// On Poll
}

#pragma warning(pop)