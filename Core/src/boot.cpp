#include "boot.h"
#include <fstream>
#include <string>
#include <SDL.h>

int main(int argc, char** argv) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) {
		std::cout << "Failed to initialize SDL.\n";
		return 1;
	}

	float memorySize = 0;

	if (argc < 3) {
		std::cout << "Usage: [Zen-1] <memSize (0-5)> <disk path>\n";
		return 1;
	}
	
	if (argv[1] == "m0") memorySize = 0.25;
	else if (argv[1] == "m1") memorySize = 0.5;
	else if (argv[1] == "m2") memorySize = 1;
	else if (argv[1] == "m3") memorySize = 2;
	else if (argv[1] == "m4") memorySize = 4;
	else if (argv[1] == "m5") memorySize = 6;
	else memorySize = 0.5;
	
	std::string storagePath = std::string(argv[2]);
	storageDevice.path = storagePath;

	std::ifstream i(storagePath, std::ios::in | std::ios::binary);
	if (!i.is_open()) {
		std::cout << "Failed to open [" << storagePath << "]\n";
		return 1;
	}

	char* buffer = new char[512];
	i.read(buffer, 512);

	initMem(memorySize);
	for (size_t i = 0; i < 512; i++)
	{
		writeMemory8(0x1000 + i, buffer[i]);
	}
	delete[] buffer;

	initComponents();
	initCPU();

	while (systemState == RUNNING) {
		std::string data;

		std::cout << "< ";
		std::getline(std::cin, data);
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (data == "regs0") {
			std::cout << "RSP: " << cpu.cores[0].regs.rsp << '\t';
			std::cout << "RBP: " << cpu.cores[0].regs.rbp << '\t';
			std::cout << "RF0: " << cpu.cores[0].regs.rf0 << '\t';
			std::cout << "RF1: " << cpu.cores[0].regs.rf1 << '\t';
			std::cout << "RF2: " << cpu.cores[0].regs.rf2 << '\t';
			std::cout << "RF3: " << cpu.cores[0].regs.rf3 << '\t';
			std::cout << "RPP: " << cpu.cores[0].regs.rf0 << '\t';
			std::cout << '\n';
			for (size_t i = 0; i < 28; i++)
			{
				std::cout << "RX" << i << " " << cpu.cores[0].regs.rx[i] << '\t';
				if (i % 7 == 0) std::cout << '\n';
			}
		}
		else if (data == "regs1") {
			std::cout << "RSP: " << cpu.cores[1].regs.rsp << '\t';
			std::cout << "RBP: " << cpu.cores[1].regs.rbp << '\t';
			std::cout << "RF0: " << cpu.cores[1].regs.rf0 << '\t';
			std::cout << "RF1: " << cpu.cores[1].regs.rf1 << '\t';
			std::cout << "RF2: " << cpu.cores[1].regs.rf2 << '\t';
			std::cout << "RF3: " << cpu.cores[1].regs.rf3 << '\t';
			std::cout << "RPP: " << cpu.cores[1].regs.rf0 << '\t';
			std::cout << '\n';
			for (size_t i = 0; i < 28; i++)
			{
				std::cout << "RX" << i << " " << cpu.cores[0].regs.rx[i] << '\t';
				if (i % 7 == 0) std::cout << '\n';
			}
		}
		else if (data == "regs2") {
			std::cout << "RSP: " << cpu.cores[2].regs.rsp << '\t';
			std::cout << "RBP: " << cpu.cores[2].regs.rbp << '\t';
			std::cout << "RF0: " << cpu.cores[2].regs.rf0 << '\t';
			std::cout << "RF1: " << cpu.cores[2].regs.rf1 << '\t';
			std::cout << "RF2: " << cpu.cores[2].regs.rf2 << '\t';
			std::cout << "RF3: " << cpu.cores[2].regs.rf3 << '\t';
			std::cout << "RPP: " << cpu.cores[2].regs.rf0 << '\t';
			std::cout << '\n';
			for (size_t i = 0; i < 28; i++)
			{
				std::cout << "RX" << i << " " << cpu.cores[2].regs.rx[i] << '\t';
				if (i % 7 == 0) std::cout << '\n';
			}
		}
		else if (data == "regs3") {
			std::cout << "RSP: " << cpu.cores[3].regs.rsp << '\t';
			std::cout << "RBP: " << cpu.cores[3].regs.rbp << '\t';
			std::cout << "RF0: " << cpu.cores[3].regs.rf0 << '\t';
			std::cout << "RF1: " << cpu.cores[3].regs.rf1 << '\t';
			std::cout << "RF2: " << cpu.cores[3].regs.rf2 << '\t';
			std::cout << "RF3: " << cpu.cores[3].regs.rf3 << '\t';
			std::cout << "RPP: " << cpu.cores[3].regs.rf0 << '\t';
			std::cout << '\n';
			for (size_t i = 0; i < 28; i++)
			{
				std::cout << "RX" << i << " " << cpu.cores[3].regs.rx[i] << '\t';
				if (i % 7 == 0) std::cout << '\n';
			}
		}
		else if (data == "regs4") {
			std::cout << "RSP: " << cpu.cores[4].regs.rsp << '\t';
			std::cout << "RBP: " << cpu.cores[4].regs.rbp << '\t';
			std::cout << "RF0: " << cpu.cores[4].regs.rf0 << '\t';
			std::cout << "RF1: " << cpu.cores[4].regs.rf1 << '\t';
			std::cout << "RF2: " << cpu.cores[4].regs.rf2 << '\t';
			std::cout << "RF3: " << cpu.cores[4].regs.rf3 << '\t';
			std::cout << "RPP: " << cpu.cores[4].regs.rf0 << '\t';
			std::cout << '\n';
			for (size_t i = 0; i < 28; i++)
			{
				std::cout << "RX" << i << " " << cpu.cores[4].regs.rx[i] << '\t';
				if (i % 7 == 0) std::cout << '\n';
			}
		}
		else if (data == "regs5") {
			std::cout << "RSP: " << cpu.cores[5].regs.rsp << '\t';
			std::cout << "RBP: " << cpu.cores[5].regs.rbp << '\t';
			std::cout << "RF0: " << cpu.cores[5].regs.rf0 << '\t';
			std::cout << "RF1: " << cpu.cores[5].regs.rf1 << '\t';
			std::cout << "RF2: " << cpu.cores[5].regs.rf2 << '\t';
			std::cout << "RF3: " << cpu.cores[5].regs.rf3 << '\t';
			std::cout << "RPP: " << cpu.cores[5].regs.rf0 << '\t';
			std::cout << '\n';
			for (size_t i = 0; i < 28; i++)
			{
				std::cout << "RX" << i << " " << cpu.cores[5].regs.rx[i] << '\t';
				if (i % 7 == 0) std::cout << '\n';
			}
		}
		else if (data == "regs6") {
			std::cout << "RSP: " << cpu.cores[6].regs.rsp << '\t';
			std::cout << "RBP: " << cpu.cores[6].regs.rbp << '\t';
			std::cout << "RF0: " << cpu.cores[6].regs.rf0 << '\t';
			std::cout << "RF1: " << cpu.cores[6].regs.rf1 << '\t';
			std::cout << "RF2: " << cpu.cores[6].regs.rf2 << '\t';
			std::cout << "RF3: " << cpu.cores[6].regs.rf3 << '\t';
			std::cout << "RPP: " << cpu.cores[6].regs.rf0 << '\t';
			std::cout << '\n';
			for (size_t i = 0; i < 28; i++)
			{
				std::cout << "RX" << i << " " << cpu.cores[6].regs.rx[i] << '\t';
				if (i % 7 == 0) std::cout << '\n';
			}
		}
		else if (data == "regs7") {
			std::cout << "RSP: " << cpu.cores[7].regs.rsp << '\t';
			std::cout << "RBP: " << cpu.cores[7].regs.rbp << '\t';
			std::cout << "RF0: " << cpu.cores[7].regs.rf0 << '\t';
			std::cout << "RF1: " << cpu.cores[7].regs.rf1 << '\t';
			std::cout << "RF2: " << cpu.cores[7].regs.rf2 << '\t';
			std::cout << "RF3: " << cpu.cores[7].regs.rf3 << '\t';
			std::cout << "RPP: " << cpu.cores[7].regs.rf0 << '\t';
			std::cout << '\n';
			for (size_t i = 0; i < 28; i++)
			{
				std::cout << "RX" << i << " " << cpu.cores[7].regs.rx[i] << '\t';
				if (i % 7 == 0) std::cout << '\n';
			}
		}
		else if (data == "pause") {
			cpu.cores[0].pause = true;
			cpu.cores[1].pause = true;
			cpu.cores[2].pause = true;
			cpu.cores[3].pause = true;
			cpu.cores[4].pause = true;
			cpu.cores[5].pause = true;
			cpu.cores[6].pause = true;
			cpu.cores[7].pause = true;
		}
		else if (data == "stepi") {
			cpu.cores[0].stepi = true;
			cpu.cores[1].stepi = true;
			cpu.cores[2].stepi = true;
			cpu.cores[3].stepi = true;
			cpu.cores[4].stepi = true;
			cpu.cores[5].stepi = true;
			cpu.cores[6].stepi = true;
			cpu.cores[7].stepi = true;
		}
		else if (data == "c") {
			cpu.cores[0].pause = false;
			cpu.cores[1].pause = false;
			cpu.cores[2].pause = false;
			cpu.cores[3].pause = false;
			cpu.cores[4].pause = false;
			cpu.cores[5].pause = false;
			cpu.cores[6].pause = false;
			cpu.cores[7].pause = false;
			cpu.cores[0].stepi = false;
			cpu.cores[1].stepi = false;
			cpu.cores[2].stepi = false;
			cpu.cores[3].stepi = false;
			cpu.cores[4].stepi = false;
			cpu.cores[5].stepi = false;
			cpu.cores[6].stepi = false;
			cpu.cores[7].stepi = false;
		}
	}

	delete[] cpu.cpu_threads;
	delete[] memory.mem1;
	delete[] memory.mem2;
	delete[] memory.mem3;
	delete[] memory.mem4;
	delete[] memory.mem5;
	delete[] memory.mem6;
	delete[] memory.mem7;
	delete[] memory.mem8;

	return 0;
}