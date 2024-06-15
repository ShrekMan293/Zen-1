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
				// TODO: Invalid Operand Interrupt
				return;
			}

			cpu.cores[index].regs.rx[op1] &= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 1: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				// TODO: Invalid Operand Interrupt
				return;
			}

			cpu.cores[index].regs.rx[op1] |= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 2: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				// TODO: Invalid Operand Interrupt
				return;
			}

			cpu.cores[index].regs.rx[op1] = ~cpu.cores[index].regs.rx[op2];
			break;
		}
		case 3: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				// TODO: Invalid Operand Interrupt
				return;
			}

			cpu.cores[index].regs.rx[op1] = ~(cpu.cores[index].regs.rx[op1] | cpu.cores[index].regs.rx[op2]);
			break;
		}
		case 4: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				// TODO: Invalid Operand Interrupt
				return;
			}

			cpu.cores[index].regs.rx[op1] ^= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 5: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				// TODO: Invalid Operand Interrupt
				return;
			}

			if (cpu.cores[index].regs.rx[op1] == cpu.cores[index].regs.rx[op2]) {
				cpu.cores[index].regs.rf5 |= 0b00000001;
			}
			else if (cpu.cores[index].regs.rx[op1] > cpu.cores[index].regs.rx[op2]) {
				cpu.cores[index].regs.rf5 |= 0b00001000;
			}
			else if (cpu.cores[index].regs.rx[op1] < cpu.cores[index].regs.rx[op2]) {
				cpu.cores[index].regs.rf5 |= 0b00100000;
			}	// Else case is impossible
			break;
		}
		default:	// TODO: Invalid Instruction Interrupt
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
				// TODO: Invalid Operand Interrupt
				return;
			}

			cpu.cores[index].regs.rx[op1] += cpu.cores[index].regs.rx[op2];
			break;
		}
		case 1: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				// TODO: Invalid Operand Interrupt
				return;
			}

			cpu.cores[index].regs.rx[op1] -= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 2: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				// TODO: Invalid Operand Interrupt
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
				// TODO: Invalid Operand Interrupt
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
				// TODO: Invalid Operand Interrupt
				return;
			}

			cpu.cores[index].regs.rx[op1] <<= cpu.cores[index].regs.rx[op2];
			break;
		}
		case 5: {
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
			fetchMemory(cpu.cores[index].regs.rpp++, 1, &op2, 0);

			if (op1 > 27 || op2 > 27) {
				// TODO: Invalid Operand Interrupt
				return;
			}

			cpu.cores[index].regs.rx[op1] >>= cpu.cores[index].regs.rx[op2];
			break;
		}
		default:	// TODO: Invalid Instruction Interrupt
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
					cpu.cores[index].regs.rx[op1] = cpu.cores[index].regs.rx[op2];
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

					fetchMemory(cpu.cores[index].regs.rx[op2], bitLength / 8, &cpu.cores[index].regs.rx[op1], 0);
					break;
				}
				default: // Invalid Instruction
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

					fetchMemory(op2, bitLength / 8, &cpu.cores[index].regs.rx[op1], 0);
					break;
				}
				case 3: {
					cpu.cores[index].regs.rx[op1] = op2;
					break;
				}
				default: // Invalid Instruction
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
				default: // Invalid Instruction
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
				default: // Invalid Instruction
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
					writeMemory64(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rx[op1]);
				}
				else {
					fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
					writeMemory32(cpu.cores[index].regs.rsp, cpu.cores[index].regs.rx[op1] & 0xFFFF);
				}
				break;
			}
			case 5: {
				if (bitMode == 64) {
					fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
					fetchMemory(cpu.cores[index].regs.rsp, 8, &cpu.cores[index].regs.rx[op1], 0);
				}
				else {
					fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
					fetchMemory(cpu.cores[index].regs.rsp, 4, &cpu.cores[index].regs.rx[op1], 0);
				}
				break;
			}
			case 8:
			default:	// TODO: Invalid Instruction Interrupt
				return;
			}
		}
		else if ((inst & 0b0000000101000000) == 0b0000000101000000) {	// Conditionals
			inst &= 0xF;

			switch (inst)
			{
			case 0: {
				if ((cpu.cores[index].regs.rf5 & 1) == 0) {	// Gets bit 0 alone and checks
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
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
				if (cpu.cores[index].regs.rf5 & 0b00000010) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
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
				if (cpu.cores[index].regs.rf5 & 0b00100000) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
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
				if (cpu.cores[index].regs.rf5 & 0b00100000 || (cpu.cores[index].regs.rf5 & 1) == 0) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
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
				if (cpu.cores[index].regs.rf5 & 0b00001000) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
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
				if (cpu.cores[index].regs.rf5 & 0b00001000 || (cpu.cores[index].regs.rf5 & 1) == 0) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
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
				if (cpu.cores[index].regs.rf5 & 0b00000100) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
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
				if (cpu.cores[index].regs.rf5 & 0b00000001) {
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
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
				if (((cpu.cores[index].regs.rf5 >> 1) & 1) == 0) {	// Gets bit 0 alone and checks
					if (inst & 0b0000100000000000) {
						fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
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
				if (inst & 0b100000000000) {
					fetchMemory(cpu.cores[index].regs.rpp++, 1, &op1, 0);
					cpu.cores[index].regs.rx[op1] = cpu.cores[index].regs.rf5;
				}
				else {
					if (bitMode == 64) {
						fetchMemory(cpu.cores[index].regs.rpp, 8, &op1, 0);
						cpu.cores[index].regs.rpp += 8;
						writeMemory8(op1, cpu.cores[index].regs.rf5);
					}
					else {
						fetchMemory(cpu.cores[index].regs.rpp, 4, &op1, 0);
						cpu.cores[index].regs.rpp += 4;
						writeMemory8(op1, cpu.cores[index].regs.rf5);
					}
				}
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
			uint64_t address = 0;

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
		else if (inst == 0x0009) {	// Interrupt Return
			// TODO: Interrupt Calls
			return;
		}
		else {
			// TODO: Invalid Instruction Interrupt
			return;
		}
	}
	else {
		// TODO: Invalid Instruction Interrupt
		return;
	}
}

void ioParse(word inst, byte index) {
	if (inst == 0) return;	// NOP
	// TODO: Add Interrupt and Gate Control
}

void runCore(byte index) {
	byte bitMode = 64;

	writeMemory16(0x1000, 0x3BC0);
	writeMemory8(0x1002, 0);
	writeMemory64(0x1003, 415);

	writeMemory16(0x100B, 0x3BC0);
	writeMemory8(0x100D, 1);
	writeMemory64(0x100E, 150);

	writeMemory16(0x1016, 0x0791);
	writeMemory8(0x1018, 0);
	writeMemory8(0x1019, 1);

	writeMemory16(0x101A, 0x0951);
	writeMemory8(0x101C, 0);

	word inst = 0x0000;

	while (cpu.cores[index].power == RUNNING) {
		fetchMemory(cpu.cores[index].regs.rpp, 2, &inst, 0);
		cpu.cores[index].regs.rpp += 2;
		if ((inst & 0x0700) == 0x0700) {
			aluParse(inst, index);
		}
		else if ((inst & 0x0100) == 0x0100 || (inst & 0x0040) == 0x0040) {
			internalParse(inst, index, bitMode);
		}
		else ioParse(inst, index);
	}
}

static void idle(byte index) {
	while (cpu.cores[index].power == IDLE);
}

static void cpuMain(byte index) {
	while (cpu.cores[index].power != DOWN) {
		if (cpu.cores[index].power == IDLE) {
			idle(index);
		}
		else runCore(index);
	}
}

void initCPU() {
	cpu.cpu_threads = new std::thread[8];

	for (byte i = 0; i < 8; i++) {
		cpu.cores[i].cpuid = (byte)i;
		if (i == 0) cpu.cores[i].power = RUNNING;
		else {
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
		cpu.cores[i].regs.rf4 = 0;
		cpu.cores[i].regs.rf5 = 0;
		cpu.cores[i].regs.pdr = 0;
	}

	cpu.cpu_threads[0] = thread(cpuMain, 0);
}