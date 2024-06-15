#include "mobo.h"
#include "cpu.h"
#include <fstream>
#include <filesystem>

std::uintmax_t fileSize(std::string path) {
	std::filesystem::path filePath = path;
	std::uintmax_t fileSize = std::filesystem::file_size(filePath);

	return fileSize;
}

motherboardRequest request = { 0 };
interruptRequest irq = { 0 };

mobo_t controller = { (pm)0 };

mobo_t storage = { (pm)0 };
storage_t storageDevice = { 0 };

mobo_t audio = { (pm)0 };
mobo_t graphics = { (pm)0 };
mobo_t timer = { (pm)0 };
mobo_t rtc = { (pm)0 };
mobo_t usb = { (pm)0 };
mobo_t rng = { (pm)0 };
//mobo_t network = { (pm)0 };
mobo_t scu = { (pm)0 };

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
				i.read(data, count * 512);
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
					i.read(data, count * 512);
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
				int numSectors = diskSize / 512;

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
			if (request.op != read);	// TODO: Non-Readable Gate Interrupt
			else cpu.cores[request.core].regs.rx[request.data] = storageDevice.status;
		}
	}
	if (storage.info.power == IDLE || !storageDevice.is_on) {
		while (storage.info.power == IDLE || !storageDevice.is_on);
		goto storage_top;	// I know. I'm sorry
	}
	else storageDevice.status = 0xD0;
}

void graphicsMain() {

}

void timerMain() {

}

void usbMain() {

}

void onPollMain(mobo_t* device, byte bitSize) {
	switch (device->info.base)
	{
	case 0x11A0: {	// RTC

	}
	case 0x9000: {	// RNG

	}
	case 0x0000: {	// SCU
		switch (request.gate)
		{
		case 0x0000: {
			if (request.op == read) cpu.cores[request.core].regs.rx[request.data] = 
			break;
		}
		case 0x00C0:
		case 0x00D0:
		default:
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
				onPollMain(&scu, 8);
				break;
			}
			case 0x00C0: {
				onPollMain(&scu, 8);
				break;
			}
			case 0x00D0: {
				onPollMain(&scu, 64);
				break;
			}
			default:
				break;
			}
		}
		else {

		}
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

void createIRQ(word gate, byte value, size_t* registers, uint64_t* data, byte registerCount)
{
}

void initComponents() {
	controller.info.power = RUNNING;
	controller.info.base = 0x0000;
	controller.info.max = 0x0000;
	controller.info.DMA = 0;
	controller.info.id = 0x00;
	controller.thread = std::thread(motherboardMain);

	storage.info.power = IDLE;
	storage.info.base = 0x4D00;
	storage.info.max = 0x4D09;
	storage.info.DMA = 1;
	storage.info.id = 0x80;
	storage.thread = std::thread(storageMain);

	audio.info.power = IDLE;
	audio.info.base = 0x3CE0;
	audio.info.max = 0x3CE8;
	audio.info.DMA = 1;
	audio.info.id = 0xE0;
	audio.thread = std::thread(audioMain);

	graphics.info.power = RUNNING;
	graphics.info.base = 0xD200;
	graphics.info.max = 0xD204;
	graphics.info.DMA = 1;
	graphics.info.id = 0xB0;
	graphics.thread = std::thread(graphicsMain);

	timer.info.power = IDLE;
	timer.info.base = 0xC000;
	timer.info.max = 0xC00A;
	timer.info.DMA = 0;
	timer.info.id = 0x20;
	timer.thread = std::thread(timerMain);

	rtc.info.power = RUNNING;
	rtc.info.base = 0x11A0;
	rtc.info.max = 0x11A9;
	rtc.info.DMA = 0;
	rtc.info.id = 0x21;
	rtc.thread;	// On Poll

	usb.info.power = IDLE;
	usb.info.base = 0xB800;
	usb.info.max = 0xB80A;
	usb.info.DMA = 1;
	usb.info.id = 0x60;
	usb.thread = std::thread(usbMain);

	scu.info.power = RUNNING;
	scu.info.base = 0x0000;
	scu.info.max = 0x0D08;
	scu.info.DMA = 1;
	scu.info.id = 0x01;
	scu.thread;	// On Poll
}