#include "cpu.h"


cpu_controller cpu = { (pm)0, NULL };


void aluParse(word inst, byte index) {
	byte op1 = 0;
	byte op2 = 0;

	if ((inst & 0b0000010001000000) == 0b0000010001000000) {	// Logic Operations
		inst -= 0x7D6;
		switch (inst)
		{
		case 0: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			cpu.cores[index].regs.rx[op1] &= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 1: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			cpu.cores[index].regs.rx[op1] |= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 2: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			cpu.cores[index].regs.rx[op1] = ~cpu.cores[index].regs.rx[op2];
			break;
		}
		case 3: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			cpu.cores[index].regs.rx[op1] = ~(cpu.cores[index].regs.rx[op1] | cpu.cores[index].regs.rx[op2]);
			break;
		}
		case 4: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			cpu.cores[index].regs.rx[op1] ^= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 5: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			if (cpu.cores[index].regs.rx[op1] == cpu.cores[index].regs.rx[op2]) {
				cpu.cores[index].regs.rf3 |= 0b00000001;
			}
			else if (cpu.cores[index].regs.rx[op1] > cpu.cores[index].regs.rx[op2]) {
				cpu.cores[index].regs.rf3 |= 0b00001000;
			}
			else if (cpu.cores[index].regs.rx[op1] < cpu.cores[index].regs.rx[op2]) {
				cpu.cores[index].regs.rf3 |= 0b00100000;
			}	// Else case is impossible
			break;
		}
		default:
			size_t* registers = new size_t[1];
			uint64_t* data = new uint64_t[1];
			registers[0] = 4;
			data[0] = cpu.cores[index].regs.rpp;
			createIRQ(index, 0, 0x02, registers, data, 1);
			return;
		}
	}
	else {	// Math Operations
		inst -= 0x790;
		switch (inst)
		{
		case 0: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			cpu.cores[index].regs.rx[op1] += cpu.cores[index].regs.rx[op2];
			break;
		}
		case 1: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			cpu.cores[index].regs.rx[op1] -= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 2: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			if (cpu.cores[index].regs.rx[op2] == 0) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x00, registers, data, 1);
				return;
			}

			cpu.cores[index].regs.rx[op1] *= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 3: {
			byte op3;

			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			if (op2 == 27) op3 = 0;
			else op3 = op2 + 1;

			cpu.cores[index].regs.rx[op3] = cpu.cores[index].regs.rx[op1] % cpu.cores[index].regs.rx[op2];
			cpu.cores[index].regs.rx[op1] /= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 4: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			cpu.cores[index].regs.rx[op1] <<= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 5: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x01, registers, data, 1);
				return;
			}

			cpu.cores[index].regs.rx[op1] >>= cpu.cores[index].regs.rx[op2];
			break;
		}
		default:
			size_t* registers = new size_t[1];
			uint64_t* data = new uint64_t[1];
			registers[0] = 4;
			data[0] = cpu.cores[index].regs.rpp;
			createIRQ(index, 0, 0x02, registers, data, 1);
			return;
		}
	}
}

void internalParse(word inst, byte index, byte bitMode) {
	uint64_t op1 = 0;
	uint64_t op2 = 0;
	byte bitLength = 64;

	if ((inst & 0b0000001100000000) == 0b0000001100000000) {	// MOV
		if (inst & 0b0000100000000000) {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			if (inst & 0b0000000000010000) {
				fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);
				switch ((inst & 0x3000) >> 12)
				{
				case 0: {
					if (op1 > 27) {
						switch (op1) {
						case 28: cpu.cores[index].regs.rsp = cpu.cores[index].regs.rx[op2]; break;
						case 29: cpu.cores[index].regs.rbp = cpu.cores[index].regs.rx[op2]; break;
						case 30: cpu.cores[index].regs.rf0 = (byte)cpu.cores[index].regs.rx[op2]; break;
						case 31: cpu.cores[index].regs.rf1 = cpu.cores[index].regs.rx[op2]; break;
						case 32: cpu.cores[index].regs.rf2 = cpu.cores[index].regs.rx[op2]; break;
						case 33: cpu.cores[index].regs.rf3 = (byte)cpu.cores[index].regs.rx[op2]; break;
						default: size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							return;
						}
					}
					else cpu.cores[index].regs.rx[op1] = cpu.cores[index].regs.rx[op2];
					if (op2 > 27) {
						switch (op2) {
						case 28: cpu.cores[index].regs.rsp = cpu.cores[index].regs.rx[op2]; break;
						case 29: cpu.cores[index].regs.rbp = cpu.cores[index].regs.rx[op2]; break;
						case 30: cpu.cores[index].regs.rf0 = (byte)cpu.cores[index].regs.rx[op2]; break;
						case 31: cpu.cores[index].regs.rf1 = cpu.cores[index].regs.rx[op2]; break;
						case 32: cpu.cores[index].regs.rf2 = cpu.cores[index].regs.rx[op2]; break;
						case 33: cpu.cores[index].regs.rf3 = (byte)cpu.cores[index].regs.rx[op2]; break;
						default: size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							return;
						}
					}
					break;
				}
				case 1: {
					switch (inst >> 14)
					{
					case 0: break;
					case 1: bitLength = 32; break;
					case 2: bitLength = 16; break;
					case 3: bitLength = 8; break;
					}

					if (op1 > 27) {
						switch (op1) {
						case 28: fetchMemory(cpu.cores[index].regs.rx[op2], bitLength / 8,
							&cpu.cores[index].regs.rsp, 0); break;
						case 29: fetchMemory(cpu.cores[index].regs.rx[op2], bitLength / 8,
							&cpu.cores[index].regs.rbp, 0); break;
						case 30: fetchMemory(cpu.cores[index].regs.rx[op2], 1,
							&cpu.cores[index].regs.rf0, 0); break;
						case 31: fetchMemory(cpu.cores[index].regs.rx[op2], bitLength / 8,
							&cpu.cores[index].regs.rf1, 0); break;
						case 32: fetchMemory(cpu.cores[index].regs.rx[op2], bitLength / 8,
							&cpu.cores[index].regs.rf2, 0); break;
						case 33: fetchMemory(cpu.cores[index].regs.rx[op2], 1,
							&cpu.cores[index].regs.rf3, 0); break;
						default: size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							break;
						}
					}
					else fetchMemory(cpu.cores[index].regs.rx[op2], bitLength / 8, &cpu.cores[index].regs.rx[op1], 0);
					break;
				}
				default:
					size_t* registers = new size_t[1];
					uint64_t* data = new uint64_t[1];
					registers[0] = 4;
					data[0] = cpu.cores[index].regs.rpp;
					createIRQ(index, 0, 0x02, registers, data, 1);
					return;
				}
			}
			else {
				switch (inst >> 14)
				{
				case 0: break;
				case 1: bitLength = 32; break;
				case 2: bitLength = 16; break;
				case 3: bitLength = 8; break;
				}

				fetchMemory(cpu.cores[index].regs.rpp, bitLength / 8, &op2, 0);
				cpu.cores[index].regs.rpp += bitLength / 8;
				switch ((inst & 0xF000) >> 12)
				{
				case 2: {
					switch (inst >> 14)
					{
					case 0: break;
					case 1: bitLength = 32; break;
					case 2: bitLength = 16; break;
					case 3: bitLength = 8; break;
					}

					if (op1 > 27) {
						size_t* registers = new size_t[1];
						uint64_t* data = new uint64_t[1];
						registers[0] = 4;
						data[0] = cpu.cores[index].regs.rpp;
						createIRQ(index, 0, 0x01, registers, data, 1);
						return;
					}
					fetchMemory(op2, bitLength / 8, &cpu.cores[index].regs.rx[op1], 0);
					break;
				}
				case 3: {
					if (op1 > 27) {
						size_t* registers = new size_t[1];
						uint64_t* data = new uint64_t[1];
						registers[0] = 4;
						data[0] = cpu.cores[index].regs.rpp;
						createIRQ(index, 0, 0x01, registers, data, 1);
						return;
					}
					cpu.cores[index].regs.rx[op1] = op2;
					break;
				}
				default:
					size_t* registers = new size_t[1];
					uint64_t* data = new uint64_t[1];
					registers[0] = 4;
					data[0] = cpu.cores[index].regs.rpp;
					createIRQ(index, 0, 0x02, registers, data, 1);
					return;
				}
			}
		}
		else {
			switch (inst >> 14)
			{
			case 0: break;
			case 1: bitLength = 32; break;
			case 2: bitLength = 16; break;
			case 3: bitLength = 8; break;
			}

			fetchMemory(cpu.cores[index].regs.rpp, bitLength / 8, &op1, 0);
			cpu.cores[index].regs.rpp += bitLength / 8;
			if (inst & 0b0000000000010000) {
				fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);
				switch ((inst & 0xF000) >> 12)
				{
				case 0: {
					if (op2 > 27) {
						size_t* registers = new size_t[1];
						uint64_t* data = new uint64_t[1];
						registers[0] = 4;
						data[0] = cpu.cores[index].regs.rpp;
						createIRQ(index, 0, 0x01, registers, data, 1);
						return;
					}

					if (bitLength == 64) {
						writeMemory64(op1, cpu.cores[index].regs.rx[op2]);
					}
					else if (bitLength == 32) {
						writeMemory32(op1, cpu.cores[index].regs.rx[op2] & 0xFFFF);
					}
					else if (bitLength == 16) {
						writeMemory16(op1, cpu.cores[index].regs.rx[op2] & 0xFF);
					}
					else {
						writeMemory8(op1, cpu.cores[index].regs.rx[op2] & 0xF);
					}
					break;
				}
				case 1: {
					switch (inst >> 14)
					{
					case 0: break;
					case 1: bitLength = 32; break;
					case 2: bitLength = 16; break;
					case 3: bitLength = 8; break;
					}
					if (op2 > 27) {
						size_t* registers = new size_t[1];
						uint64_t* data = new uint64_t[1];
						registers[0] = 4;
						data[0] = cpu.cores[index].regs.rpp;
						createIRQ(index, 0, 0x01, registers, data, 1);
						return;
					}

					if (bitLength == 64) {
						uint64_t data = 0;
						fetchMemory(cpu.cores[index].regs.rx[op2], 8, &data, 0);
						writeMemory64(op1, data);
					}
					else if (bitLength == 32) {
						uint32_t data = 0;
						fetchMemory(cpu.cores[index].regs.rx[op2], 4, &data, 0);
						writeMemory32(op1, cpu.cores[index].regs.rx[op2] & 0xFFFF);
					}
					else if (bitLength == 16) {
						word data = 0;
						fetchMemory(cpu.cores[index].regs.rx[op2], 2, &data, 0);
						writeMemory16(op1, cpu.cores[index].regs.rx[op2] & 0xFF);
					}
					else {
						byte data = 0;
						fetchMemory(cpu.cores[index].regs.rx[op2], 1, &data, 0);
						writeMemory8(op1, cpu.cores[index].regs.rx[op2] & 0xF);
					}
					break;
				}
				default:
					size_t* registers = new size_t[1];
					uint64_t* data = new uint64_t[1];
					registers[0] = 4;
					data[0] = cpu.cores[index].regs.rpp;
					createIRQ(index, 0, 0x02, registers, data, 1);
					return;
				}
			}
			else {
				switch (inst >> 14)
				{
				case 0: break;
				case 1: bitLength = 32; break;
				case 2: bitLength = 16; break;
				case 3: bitLength = 8; break;
				}

				fetchMemory(cpu.cores[index].regs.rpp, bitLength / 8, &op2, 0);
				cpu.cores[index].regs.rpp += bitLength / 8;
				switch ((inst & 0xF000) >> 12)
				{
				case 2: {
					if (bitLength == 64) {
						uint64_t data = 0;
						fetchMemory(op2, 8, &data, 0);
						writeMemory64(op1, data);
					}
					else if (bitLength == 32) {
						uint32_t data = 0;
						fetchMemory(op2, 4, &data, 0);
						writeMemory32(op1, data);
					}
					else if (bitLength == 16) {
						word data = 0;
						fetchMemory(op2, 2, &data, 0);
						writeMemory16(op1, data);
					}
					else {
						byte data = 0;
						fetchMemory(op2, 1, &data, 0);
						writeMemory8(op1, data);
					}
					break;
				}
				case 3: {
					if (bitLength == 64) {
						writeMemory64(op1, op2);
					}
					else if (bitLength == 32) {
						writeMemory32(op1, op2 & 0xFFFF);
					}
					else if (bitLength == 16) {
						writeMemory16(op1, op2 & 0xFF);
					}
					else {
						writeMemory8(op1, op2 & 0xFF);
					}
					break;
				}
				default:
					size_t* registers = new size_t[1];
					uint64_t* data = new uint64_t[1];
					registers[0] = 4;
					data[0] = cpu.cores[index].regs.rpp;
					createIRQ(index, 0, 0x02, registers, data, 1);
					return;
				}
			}
		}
	}
	else if (inst & 0b0000000100000000) {	// Single operand
		if ((inst & 0b0000000101010000) == 0b0000000101010000) {	// Memory operations
			uint64_t op1 = 0;

			switch (inst & 0xF)
			{
			case 1: {
				if (inst & 0b0000100000000000) {
					fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
					if (op1 > 27) {
						size_t* registers = new size_t[1];
						uint64_t* data = new uint64_t[1];
						registers[0] = 4;
						data[0] = cpu.cores[index].regs.rpp;
						createIRQ(index, 0, 0x01, registers, data, 1);
						return;
					}
					cpu.cores[index].regs.rpp = cpu.cores[index].regs.rx[op1];
				}
				else {
					if (bitMode == 64) {
						fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
						cpu.cores[index].regs.rpp = op1;
					}
					else {
						fetchMemory(cpu.cores[index].regs.rpp, 4, &op1, 0);
						cpu.cores[index].regs.rpp = op1;
					}
				}
				break;
			}
			case 2: {
				if (inst & 0b100000000000) {
					fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
					if (bitMode == 64) {
						writeMemory64(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rpp);
					}
					else {
						writeMemory32(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rpp & 0xFFFF);
					}

					cpu.cores[index].regs.rpp = cpu.cores[index].regs.rx[op1];
				}
				else {
					if (bitMode == 64) {
						fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
						cpu.cores[index].regs.rpp += 8;
						writeMemory64(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rpp);
					}
					else {
						fetchMemory(cpu.cores[index].regs.rpp, 4, &op1, 0);
						cpu.cores[index].regs.rpp += 4;
						writeMemory32(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rpp & 0xFFFF);
					}

					cpu.cores[index].regs.rpp = op1;
				}
				break;
			}
			case 4: {
				if (bitMode == 64) {
					fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
					if (op1 > 27) {
						size_t* registers = new size_t[1];
						uint64_t* data = new uint64_t[1];
						registers[0] = 4;
						data[0] = cpu.cores[index].regs.rpp;
						createIRQ(index, 0, 0x01, registers, data, 1);
						return;
					}
					writeMemory64(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rx[op1]);
				}
				else {
					fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
					if (op1 > 27) {
						size_t* registers = new size_t[1];
						uint64_t* data = new uint64_t[1];
						registers[0] = 4;
						data[0] = cpu.cores[index].regs.rpp;
						createIRQ(index, 0, 0x01, registers, data, 1);
						return;
					}
					writeMemory32(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rx[op1] & 0xFFFF);
				}
				break;
			}
			case 5: {
				if (bitMode == 64) {
					fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
					if (op1 > 27) {
						size_t* registers = new size_t[1];
						uint64_t* data = new uint64_t[1];
						registers[0] = 4;
						data[0] = cpu.cores[index].regs.rpp;
						createIRQ(index, 0, 0x01, registers, data, 1);
						return;
					}
					fetchMemory(cpu.cores[index].regs.rsp, 8, &cpu.cores[index].regs.rx[op1], 0);
				}
				else {
					fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
					if (op1 > 27) {
						size_t* registers = new size_t[1];
						uint64_t* data = new uint64_t[1];
						registers[0] = 4;
						data[0] = cpu.cores[index].regs.rpp;
						createIRQ(index, 0, 0x01, registers, data, 1);
						return;
					}
					fetchMemory(cpu.cores[index].regs.rsp, 4, &cpu.cores[index].regs.rx[op1], 0);
				}
				break;
			}
			default:
				size_t* registers = new size_t[1];
				uint64_t* data = new uint64_t[1];
				registers[0] = 4;
				data[0] = cpu.cores[index].regs.rpp;
				createIRQ(index, 0, 0x02, registers, data, 1);
				return;
			}
		}
		else if ((inst & 0b0000000101000000) == 0b0000000101000000) {	// Conditionals
			inst &= 0xF;

			switch (inst)
			{
			case 0: {
				if ((cpu.cores[index].regs.rf3 & 1) == 0) {	// Gets bit 0 alone and checks
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
						if (op1 > 27) {
							size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							return;
						}
						cpu.cores[index].regs.rpp = cpu.cores[index].regs.rx[op1];
					}
					else {
						if (bitMode == 64) {
							fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
							cpu.cores[index].regs.rpp += 8;
							cpu.cores[index].regs.rpp = op1;
						}
						else {
							fetchMemory(cpu.cores[index].regs.rpp, 4, &op1, 0);
							cpu.cores[index].regs.rpp += 4;
							cpu.cores[index].regs.rpp = op1;
						}
					}
				}
				break;
			}
			case 1: {
				if (cpu.cores[index].regs.rf3 & 0b00000010) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
						if (op1 > 27) {
							size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							return;
						}
						cpu.cores[index].regs.rpp = cpu.cores[index].regs.rx[op1];
					}
					else {
						if (bitMode == 64) {
							fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
							cpu.cores[index].regs.rpp += 8;
							cpu.cores[index].regs.rpp = op1;
						}
						else {
							fetchMemory(cpu.cores[index].regs.rpp, 4, &op1, 0);
							cpu.cores[index].regs.rpp += 4;
							cpu.cores[index].regs.rpp = op1;
						}
					}
				}
				break;
			}
			case 2: {
				if (cpu.cores[index].regs.rf3 & 0b00100000) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
						if (op1 > 27) {
							size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							return;
						}
						cpu.cores[index].regs.rpp = cpu.cores[index].regs.rx[op1];
					}
					else {
						if (bitMode == 64) {
							fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
							cpu.cores[index].regs.rpp += 8;
							cpu.cores[index].regs.rpp = op1;
						}
						else {
							fetchMemory(cpu.cores[index].regs.rpp, 4, &op1, 0);
							cpu.cores[index].regs.rpp += 4;
							cpu.cores[index].regs.rpp = op1;
						}
					}
				}
				break;
			}
			case 3: {
				if (cpu.cores[index].regs.rf3 & 0b00100000 || (cpu.cores[index].regs.rf3 & 1) == 0) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
						if (op1 > 27) {
							size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							return;
						}
						cpu.cores[index].regs.rpp = cpu.cores[index].regs.rx[op1];
					}
					else {
						if (bitMode == 64) {
							fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
							cpu.cores[index].regs.rpp += 8;
							cpu.cores[index].regs.rpp = op1;
						}
						else {
							fetchMemory(cpu.cores[index].regs.rpp, 4, &op1, 0);
							cpu.cores[index].regs.rpp += 4;
							cpu.cores[index].regs.rpp = op1;
						}
					}
				}
				break;
			}
			case 4: {
				if (cpu.cores[index].regs.rf3 & 0b00001000) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
						if (op1 > 27) {
							size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							return;
						}
						cpu.cores[index].regs.rpp = cpu.cores[index].regs.rx[op1];
					}
					else {
						if (bitMode == 64) {
							fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
							cpu.cores[index].regs.rpp += 8;
							cpu.cores[index].regs.rpp = op1;
						}
						else {
							fetchMemory(cpu.cores[index].regs.rpp, 4, &op1, 0);
							cpu.cores[index].regs.rpp += 4;
							cpu.cores[index].regs.rpp = op1;
						}
					}
				}
				break;
			}
			case 5: {
				if (cpu.cores[index].regs.rf3 & 0b00001000 || (cpu.cores[index].regs.rf3 & 1) == 0) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
						if (op1 > 27) {
							size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							return;
						}
						cpu.cores[index].regs.rpp = cpu.cores[index].regs.rx[op1];
					}
					else {
						if (bitMode == 64) {
							fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
							cpu.cores[index].regs.rpp += 8;
							cpu.cores[index].regs.rpp = op1;
						}
						else {
							fetchMemory(cpu.cores[index].regs.rpp, 4, &op1, 0);
							cpu.cores[index].regs.rpp += 4;
							cpu.cores[index].regs.rpp = op1;
						}
					}
				}
				break;
			}
			case 6: {
				if (cpu.cores[index].regs.rf3 & 0b00000100) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
						if (op1 > 27) {
							size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							return;
						}
						cpu.cores[index].regs.rpp = cpu.cores[index].regs.rx[op1];
					}
					else {
						if (bitMode == 64) {
							fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
							cpu.cores[index].regs.rpp += 8;
							cpu.cores[index].regs.rpp = op1;
						}
						else {
							fetchMemory(cpu.cores[index].regs.rpp, 4, &op1, 0);
							cpu.cores[index].regs.rpp += 4;
							cpu.cores[index].regs.rpp = op1;
						}
					}
				}
				break;
			}
			case 7: {
				if (cpu.cores[index].regs.rf3 & 0b00000001) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
						if (op1 > 27) {
							size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							return;
						}
						cpu.cores[index].regs.rpp = cpu.cores[index].regs.rx[op1];
					}
					else {
						if (bitMode == 64) {
							fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
							cpu.cores[index].regs.rpp += 8;
							cpu.cores[index].regs.rpp = op1;
						}
						else {
							fetchMemory(cpu.cores[index].regs.rpp, 4, &op1, 0);
							cpu.cores[index].regs.rpp += 4;
							cpu.cores[index].regs.rpp = op1;
						}
					}
				}
				break;
			}
			case 8: {
				if (((cpu.cores[index].regs.rf3 >> 1) & 1) == 0) {	// Gets bit 0 alone and checks
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
						if (op1 > 27) {
							size_t* registers = new size_t[1];
							uint64_t* data = new uint64_t[1];
							registers[0] = 4;
							data[0] = cpu.cores[index].regs.rpp;
							createIRQ(index, 0, 0x01, registers, data, 1);
							return;
						}
						cpu.cores[index].regs.rpp = cpu.cores[index].regs.rx[op1];
					}
					else {
						if (bitMode == 64) {
							fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
							cpu.cores[index].regs.rpp += 8;
							cpu.cores[index].regs.rpp = op1;
						}
						else {
							fetchMemory(cpu.cores[index].regs.rpp += 4, 4, &op1, 0);
							cpu.cores[index].regs.rpp += 4;
							cpu.cores[index].regs.rpp = op1;
						}
					}
				}
				break;
			}
			case 9: {
				fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
				if (op1 > 27) {
					size_t* registers = new size_t[1];
					uint64_t* data = new uint64_t[1];
					registers[0] = 4;
					data[0] = cpu.cores[index].regs.rpp;
					createIRQ(index, 0, 0x01, registers, data, 1);
					return;
				}
				cpu.cores[index].regs.rx[op1] = cpu.cores[index].regs.rf3;
				break;
			}
			default:
				break;
			}
		}
	}
	else if (inst & 0b0000000001000000) {	// No Operand
		inst -= 0x0040;
		if (inst == 0x0003) { // Return
			if (bitMode == 64) {
				fetchMemory(cpu.cores[index].regs.rsp, 8, &cpu.cores[index].regs.rpp, 0);
			}
			else {
				fetchMemory(cpu.cores[index].regs.rsp, 4, &cpu.cores[index].regs.rpp, 0);
			}
		}
		else if (inst == 0x0006) {	// Pusha
			for (size_t i = 0; i < 28; i++)
			{
				if (bitMode == 64) {
					writeMemory64(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rx[i]);
					cpu.cores[index].regs.rsp -= 8;
				}
				else {
					writeMemory32(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rx[i] & 0xFFFF);
					cpu.cores[index].regs.rsp -= 4;
				}
			}

			if (bitMode == 64) {
				writeMemory64(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rbp);
				cpu.cores[index].regs.rsp -= 8;
				writeMemory64(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rsp);
				cpu.cores[index].regs.rsp -= 8;
			}
			else {
				writeMemory32(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rbp & 0xFFFF);
				cpu.cores[index].regs.rsp -= 4;
				writeMemory32(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rsp & 0xFFFF);
				cpu.cores[index].regs.rsp -= 4;
			}
		}
		else if (inst == 0x0007) {	// Popa
			if (bitMode == 64) {
				fetchMemory(cpu.cores[index].regs.rsp, 8, &cpu.cores[index].regs.rsp, 0);
				cpu.cores[index].regs.rsp += 8;
				fetchMemory(cpu.cores[index].regs.rsp, 8, &cpu.cores[index].regs.rbp, 0);
				cpu.cores[index].regs.rsp += 8;
			}
			else {
				fetchMemory(cpu.cores[index].regs.rsp, 4, &cpu.cores[index].regs.rsp, 0);
				cpu.cores[index].regs.rsp += 4;
				fetchMemory(cpu.cores[index].regs.rsp, 4, &cpu.cores[index].regs.rbp, 0);
				cpu.cores[index].regs.rsp += 4;
			}

			for (size_t i = 28; i > 0; i--)
			{
				if (bitMode == 64) {
					fetchMemory(cpu.cores[index].regs.rsp, 8, &cpu.cores[index].regs.rx[i], 0);
					cpu.cores[index].regs.rsp += 8;
				}
				else {
					fetchMemory(cpu.cores[index].regs.rsp, 4, &cpu.cores[index].regs.rx[i], 0);
					cpu.cores[index].regs.rsp += 4;
				}
			}
		}
		else if (inst == 0x0008) {	// Interrupt Return
			if (bitMode == 64) {
				fetchMemory(cpu.cores[index].regs.rsp, 8, &cpu.cores[index].regs.rsp, 0);
				cpu.cores[index].regs.rsp += 8;
				fetchMemory(cpu.cores[index].regs.rsp, 8, &cpu.cores[index].regs.rbp, 0);
				cpu.cores[index].regs.rsp += 8;
			}
			else {
				fetchMemory(cpu.cores[index].regs.rsp, 4, &cpu.cores[index].regs.rsp, 0);
				cpu.cores[index].regs.rsp += 4;
				fetchMemory(cpu.cores[index].regs.rsp, 4, &cpu.cores[index].regs.rbp, 0);
				cpu.cores[index].regs.rsp += 4;
			}

			for (size_t i = 28; i > 0; i--)
			{
				if (bitMode == 64) {
					fetchMemory(cpu.cores[index].regs.rsp, 8, &cpu.cores[index].regs.rx[i], 0);
					cpu.cores[index].regs.rsp += 8;
				}
				else {
					fetchMemory(cpu.cores[index].regs.rsp, 4, &cpu.cores[index].regs.rx[i], 0);
					cpu.cores[index].regs.rsp += 4;
				}
			}

			if (bitMode == 64) {
				fetchMemory(cpu.cores[index].regs.rsp, 8, &cpu.cores[index].regs.rpp, 0);
			}
			else {
				fetchMemory(cpu.cores[index].regs.rsp, 4, &cpu.cores[index].regs.rpp, 0);
			}

			delete[] irq.registers;
			delete[] irq.data;
			irq = nullIRQ;
			cpu.cores[index].regs.rf0 &= 0b11011111;
			return;
		}
		else {
			size_t* registers = new size_t[1];
			uint64_t* data = new uint64_t[1];
			registers[0] = 4;
			data[0] = cpu.cores[index].regs.rpp;
			createIRQ(index, 0, 0x02, registers, data, 1);
			return;
		}
	}
	else {
		size_t* registers = new size_t[1];
		uint64_t* data = new uint64_t[1];
		registers[0] = 4;
		data[0] = cpu.cores[index].regs.rpp;
		createIRQ(index, 0, 0x02, registers, data, 1);
		return;
	}
}

void ioParse(word inst, byte index) {
	byte op1 = 0;
	byte op2 = 0;

	switch (inst) {
	case 0: return;	// NOP
	case 0x391: {	// In
		request.core = index;
		request.op = read;
		fetchMemory(cpu.cores[index].regs.rpp++, 1, &request.data, 0);
		fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);
		if (request.data > 27 || op2 > 27) {
			size_t* registers = new size_t[1];
			uint64_t* data = new uint64_t[1];
			registers[0] = 4;
			data[0] = cpu.cores[index].regs.rpp;
			createIRQ(index, 0, 0x01, registers, data, 1);
			return;
		}
		request.gate = (word)cpu.cores[index].regs.rx[op2];
		break;
	}
	case 0x03F2: {	// Out
		request.core = index;
		request.op = write;
		fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
		if (op1 > 27) {
			size_t* registers = new size_t[1];
			uint64_t* data = new uint64_t[1];
			registers[0] = 4;
			data[0] = cpu.cores[index].regs.rpp;
			createIRQ(index, 0, 0x01, registers, data, 1);
			return;
		}
		request.gate = (word)cpu.cores[index].regs.rx[op1];
		fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);
		if (op2 > 27) {
			size_t* registers = new size_t[1];
			uint64_t* data = new uint64_t[1];
			registers[0] = 4;
			data[0] = cpu.cores[index].regs.rpp;
			createIRQ(index, 0, 0x01, registers, data, 1);
			return;
		}
		request.data = cpu.cores[index].regs.rx[op2];
		break;
	}
	case 0x0113: {
		fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
		createIRQ(index, 0, op1, nullptr, nullptr, 0);
		break;
	}
	case 0x0004: {
		while ((cpu.cores[index].regs.rf0 | 0b00100000) == 0);
		break;
	}
	case 0x0005: cpu.cores[index].regs.rf0 &= 0b10111111; break;
	case 0x0006: cpu.cores[index].regs.rf0 |= 0b01000000; break;
	default:
		size_t* registers = new size_t[1];
		uint64_t* data = new uint64_t[1];
		registers[0] = 4;
		data[0] = cpu.cores[index].regs.rpp;
		createIRQ(index, 0, 0x02, registers, data, 1);
	}
}

bool IRQEqual(interruptRequest i1, interruptRequest i2) {
	if (i1.core == i2.core && i1.data == i2.data &&
		i1.gate == i2.gate && i1.registers == i2.registers &&
		i1.value == i2.value) return true;
	else return false;
}

void runCore(byte index) {
	byte bitMode = 64;

	word inst = 0x0000;
	interruptRequest currentIRQ = nullIRQ;
	byte instructionCount = 0;

	while (cpu.cores[index].power == RUNNING) {
		while (cpu.cores[index].pause);
		if (cpu.cores[index].stepi) instructionCount++;

		if ((cpu.cores[index].regs.rf0 & 0x3) == 3) bitMode = 32;
		else bitMode = 64;

		if (!IRQEqual(currentIRQ, nullIRQ) && irq.core == index) {
			for (size_t i = 0; i < 28; i++)
			{
				if (bitMode == 64) {
					writeMemory64(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rx[i]);
					cpu.cores[index].regs.rsp -= 8;
				}
				else {
					writeMemory32(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rx[i] & 0xFFFF);
					cpu.cores[index].regs.rsp -= 4;
				}
			}

			if (bitMode == 64) {
				writeMemory64(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rbp);
				cpu.cores[index].regs.rsp -= 8;
				writeMemory64(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rsp);
				cpu.cores[index].regs.rsp -= 8;
			}
			else {
				writeMemory32(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rbp & 0xFFFF);
				cpu.cores[index].regs.rsp -= 4;
				writeMemory32(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rsp & 0xFFFF);
				cpu.cores[index].regs.rsp -= 4;
			}

			if (bitMode == 64) writeMemory64(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rpp);
			else writeMemory32(cpu.cores[index].regs.rsp, (uint32_t)cpu.cores[index].regs.rpp);

			for (size_t i = 0; i < irq.regCount; i++) {
				cpu.cores[index].regs.rx[irq.registers[i]] = irq.data[i];
			}
			cpu.cores[index].regs.rx[0] = irq.gate;
			cpu.cores[index].regs.rx[1] = irq.value;

			if (irq.value == 0x07) cpu.cores[index].regs.rpp = cpu.cores[index].regs.rf2;
			
			fetchMemory(cpu.cores[index].regs.rf1 + irq.value, 8, &cpu.cores[index].regs.rpp, 0);
			if (cpu.cores[index].regs.rf0 | 0b00010000) {
				createIRQ(index, 0, 0x06, nullptr, nullptr, 0);
				cpu.cores[index].regs.rf0 = 0b10;
				request.core = index;
				request.data = 0xF0;
				request.gate = 0x00C0;
				request.op = write;
			}
			if (cpu.cores[index].regs.rf0 | 0b00100000) {
				createIRQ(index, 0, 0x05, nullptr, nullptr, 0);
				cpu.cores[index].regs.rf0 = 0b01;
			}
			cpu.cores[index].regs.rf0 |= 0b00100000;
		}

		fetchMemory(cpu.cores[index].regs.rpp, 2, &inst, 0);
		cpu.cores[index].regs.rpp += 2;
		if ((inst & 0x0700) == 0x0700) {
			aluParse(inst, index);
		}
		else if ((inst & 0x0100) == 0x0100 || (inst & 0x0040) == 0x0040) {
			internalParse(inst, index, bitMode);
		}
		else if (inst == 0x0FFF) {
			request.core = index;
			request.gate = 0x0000;
			request.op = write;
			request.data = index | 0x10;

			std::this_thread::sleep_for(std::chrono::microseconds(500));

			request.core = index;
			request.gate = 0xC0;
			request.op = write;
			request.data = 0xFF;
		}
		else ioParse(inst, index);

		if (cpu.cores[index].stepi) {
			instructionCount = 0;

			while (cpu.cores[index].stepi && cpu.cores[index].pause);
		}
	}
}

static void idle(byte index) {
	while (cpu.cores[index].power == IDLE);
}

static void cpuMain(byte index) {
	while (cpu.cores[index].power != DOWN) {
		while (!cpu.cores[index].component.enabled);

		if (cpu.cores[index].power == IDLE) {
			idle(index);
		}
		else runCore(index);
	}
}

void reInitCPU(byte i) {
	cpu.cores[i].cpuid = (byte)i;
	cpu.cores[i].component.info.id = (byte)i | 0x10;	// Just a way for mobo to be able to recognize it

	cpu.cores[i].component.info.DMA = 1;
	cpu.cores[i].component.info.base = 0x0000;
	cpu.cores[i].component.info.max = 0x0000;
	cpu.cores[i].component.enabled = true;

	if (i == 0) {
		cpu.cores[i].component.info.power = RUNNING;
		cpu.cores[i].power = RUNNING;
	}
	else {
		cpu.cores[i].component.info.power = IDLE;
		cpu.cores[i].power = IDLE;
		cpu.cpu_threads[i] = thread(cpuMain, i);
	}

	for (size_t j = 0; j < 28; j++)
	{
		cpu.cores[i].regs.rx[j] = 0;
	}
	cpu.cores[i].regs.rpp = 0x1000;
	cpu.cores[i].regs.rsp = 0x3000;
	cpu.cores[i].regs.rbp = 0x3000;
	cpu.cores[i].regs.rf0 = 0;
	cpu.cores[i].regs.rf1 = 0;
	cpu.cores[i].regs.rf2 = 0;
	cpu.cores[i].regs.rf3 = 0;
}

void initCPU() {
	cpu.cpu_threads = new std::thread[8];

	for (byte i = 0; i < 8; i++) {
		cpu.cores[i].cpuid = (byte)i;
		cpu.cores[i].component.info.id = (byte)i | 0x10;	// Just a way for mobo to be able to recognize it

		cpu.cores[i].component.info.DMA = 1;
		cpu.cores[i].component.info.base = 0x0000;
		cpu.cores[i].component.info.max = 0x0000;
		cpu.cores[i].component.enabled = true;

		if (i == 0) {
			cpu.cores[i].component.info.power = RUNNING;
			cpu.cores[i].power = RUNNING;
		}
		else {
			cpu.cores[i].component.info.power = IDLE;
			cpu.cores[i].power = IDLE;
			cpu.cpu_threads[i] = thread(cpuMain, i);
		}

		for (size_t j = 0; j < 28; j++)
		{
			cpu.cores[i].regs.rx[j] = 0;
		}
		cpu.cores[i].regs.rpp = 0x1000;
		cpu.cores[i].regs.rsp = 0x3000;
		cpu.cores[i].regs.rbp = 0x3000;
		cpu.cores[i].regs.rf0 = 0;
		cpu.cores[i].regs.rf1 = 0;
		cpu.cores[i].regs.rf2 = 0;
		cpu.cores[i].regs.rf3 = 0;
	}

	cpu.cpu_threads[0] = thread(cpuMain, 0);
}