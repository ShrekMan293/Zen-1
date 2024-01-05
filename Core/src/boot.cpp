#include "boot.h"
#include <Windows.h>

int main(int argc, char** argv) {
	initComponents();
	initCPU();

	return 0;
}