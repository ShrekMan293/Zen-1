#include "boot.h"

int main(int argc, char** argv) {
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

	initMem(memorySize);
	initCPU();
	initComponents();

	std::cout << cpu.cores[1].power << '\n';

	uint64_t i = 0;

	while (i++ < UINT64_MAX);

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