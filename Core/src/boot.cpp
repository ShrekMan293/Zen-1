#include "boot.h"
#include <fstream>
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

	while (systemState == RUNNING);

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