#include "codegen.h"

struct Codegen {
	Token* tokens;
	size_t curToken;
	size_t curByte;
	size_t curSize;
	uint8_t* output;
	Iden* idens;
	size_t idenSize;
	size_t curIden;
	uint8_t bitMode;
	Iden curSect;
	State state;
};

Codegen c;

void initCodeGen(Token* tokens) {
	c.tokens = tokens;
	c.curToken = 0;
	c.curByte = 0;
	c.bitMode = 64;
	c.idenSize = 10;
	c.idens = nullptr;
	c.curIden = 0;
	c.output = nullptr;
	c.state.errorCount = 0;
	c.state.status = PS_CLEAN;
	c.curSect.lexeme = nullptr;
	c.curSect.offset = 0;
}

void codegen_advance() {
	c.curToken++;
}

void codegen_advance(int offset) {
	c.curToken += offset;
}

Token codegen_getToken(int offset) {
	return c.tokens[c.curToken + offset];
}

Token codegen_currentToken() {
	return c.tokens[c.curToken];
}

void codegen_error(string message, Token t) {
	cout << "Error: " << message << " at line " << t.line << ", char " << t.pos << ".\n";
	c.state.errorCount++;
	c.state.status = PS_CRITICAL;
}

void writeByte(uint8_t data) {
	if (c.curByte == c.curSize - 1) {
		uint8_t* newArray = new uint8_t[c.curSize + 50];
		memcpy(newArray, c.output, c.curSize);
		c.curSize += 50;
		delete[] c.output;
		c.output = newArray;
	}

	c.output[c.curByte++] = data;
}

void writeWord(uint16_t data) {
	writeByte((uint8_t)data & 0xFF);
	writeByte((uint8_t)(data >> 8) & 0xFF);
}

void writeDWord(uint32_t data) {
	writeByte((uint8_t)data & 0xFF);
	writeByte((uint8_t)(data >> 8) & 0xFF);
	writeByte((uint8_t)(data >> 16) & 0xFF);
	writeByte((uint8_t)(data >> 24) & 0xFF);
}

void writeQWord(uint64_t data) {
	writeDWord((uint32_t)data & 0xFFFFFFFF);
	writeDWord((uint32_t)(data >> 32) & 0xFFFFFFFF);
}

uint64_t getIdenOffset(char* lexeme) {
	for (size_t i = 0; i < c.curIden; i++)
	{
		if (c.idens[i].lexeme == lexeme) return c.idens[i].offset;
	}

	return 0;
}

void add_instruction() {
	writeByte((uint8_t)0x90);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void sub_instruction() {
	writeByte((uint8_t)0x91);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void mul_instruction() {
	writeByte((uint8_t)0x92);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void div_instruction() {
	writeByte((uint8_t)0x93);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void shl_instruction() {
	writeByte((uint8_t)0x94);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void shr_instruction() {
	writeByte((uint8_t)0x95);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void and_instruction() {
	writeByte((uint8_t)0xD6);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void or_instruction() {
	writeByte((uint8_t)0xD7);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void not_instruction() {
	writeByte((uint8_t)0xD8);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void nor_instruction() {
	writeByte((uint8_t)0xD9);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void xor_instruction() {
	writeByte((uint8_t)0xDA);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void cmp_instruction() {
	writeByte((uint8_t)0xDB);
	writeByte((uint8_t)0x07);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void movq_instruction() {
	bool regOp1 = false;
	bool regOp2 = false;
	bool op2AsPtr = false;

	if (codegen_getToken(3).type == TT_REG) regOp2 = true;
	else if (codegen_getToken(3).type == TT_LBRACKET) {
		op2AsPtr = true;
		if (codegen_getToken(4).type == TT_REG) regOp2 = true;
	}

	if (codegen_getToken(1).type == TT_REG) regOp1 = true;

	if (regOp2) writeByte((uint8_t)0xD0);
	else writeByte((uint8_t)0xC0);

	if (regOp1) {
		if (regOp2) {
			if (op2AsPtr) {
				writeByte((uint8_t)0x1B);
			}
			else {
				writeByte((uint8_t)0x0B);
			}
		}
		else {
			if (op2AsPtr) {
				writeByte((uint8_t)0x2B);
			}
			else {
				writeByte((uint8_t)0x3B);
			}
		}
	}
	else {
		if (regOp2) {
			if (op2AsPtr) {
				writeByte((uint8_t)0x13);
			}
			else {
				writeByte((uint8_t)0x03);
			}
		}
		else {
			if (op2AsPtr) {
				writeByte((uint8_t)0x23);
			}
			else {
				writeByte((uint8_t)0x33);
			}
		}
	}
	codegen_advance();

	switch (codegen_currentToken().type)
	{
	case TT_IDEN: writeQWord(getIdenOffset(codegen_currentToken().lexeme)); break;
	case TT_REG: {
		int value = 0;
		if (string(codegen_currentToken().lexeme) == "rsp") value = 28;
		else if (string(codegen_currentToken().lexeme) == "rbp") value = 29;
		else if (string(codegen_currentToken().lexeme) == "rf0") value = 30;
		else if (string(codegen_currentToken().lexeme) == "rf1") value = 31;
		else if (string(codegen_currentToken().lexeme) == "rf2") value = 32;
		else if (string(codegen_currentToken().lexeme) == "rf3") value = 33;
		else value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		break;
	}
	case TT_NUMBER: {
		uint64_t value = 0;
		if (codegen_currentToken().lexeme[1] == 'x') {
			value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
			if (c.bitMode == 64) writeQWord(value);
		}
		else if (codegen_currentToken().lexeme[1] == 'b') {
			value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
			if (c.bitMode == 64) writeQWord(value);
		}
		else {
			value = std::stoi(string(codegen_currentToken().lexeme));
			if (c.bitMode == 64) writeQWord(value);
		}
		break;
	}
	default: break;
	}

	codegen_advance();
	codegen_advance();

	switch (codegen_currentToken().type)
	{
	case TT_CHAR: {
		for (size_t i = 0; codegen_currentToken().lexeme[i] != '\0'; i++)
		{
			writeByte((uint8_t)codegen_currentToken().lexeme[i]);
		}
		break;
	}
	case TT_CURRENT: writeQWord(c.curByte); break;
	case TT_IDEN: writeQWord(getIdenOffset(codegen_currentToken().lexeme)); break;
	case TT_LBRACKET:
	case TT_NUMBER: {
		uint64_t value = 0;
		if (codegen_currentToken().lexeme[1] == 'x') {
			value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
			if (c.bitMode == 64) writeQWord(value);
		}
		else if (codegen_currentToken().lexeme[1] == 'b') {
			value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
			if (c.bitMode == 64) writeQWord(value);
		}
		else {
			value = std::stoi(string(codegen_currentToken().lexeme));
			writeQWord(value);
		}
		break;
	}
	case TT_SECT_START: writeQWord(getIdenOffset(c.curSect.lexeme)); break;
	case TT_REG: {
		int value = 0;
		if (string(codegen_currentToken().lexeme) == "rsp") value = 28;
		else if (string(codegen_currentToken().lexeme) == "rbp") value = 29;
		else if (string(codegen_currentToken().lexeme) == "rf0") value = 30;
		else if (string(codegen_currentToken().lexeme) == "rf1") value = 31;
		else if (string(codegen_currentToken().lexeme) == "rf2") value = 32;
		else if (string(codegen_currentToken().lexeme) == "rf3") value = 33;
		else value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		break;
	}
	default:
		break;
	}
	codegen_advance();
}

void movd_instruction() {
	bool regOp1 = false;
	bool regOp2 = false;
	bool op2AsPtr = false;

	if (codegen_getToken(3).type == TT_REG) regOp2 = true;
	else if (codegen_getToken(3).type == TT_LBRACKET) {
		op2AsPtr = true;
		if (codegen_getToken(4).type == TT_REG) regOp2 = true;
	}

	if (codegen_getToken(1).type == TT_REG) regOp1 = true;

	if (regOp2) writeByte((uint8_t)0xD0);
	else writeByte((uint8_t)0xC0);

	if (regOp1) {
		if (regOp2) {
			if (op2AsPtr) {
				writeByte((uint8_t)0x1B);
			}
			else {
				writeByte((uint8_t)0x0B);
			}
		}
		else {
			if (op2AsPtr) {
				writeByte((uint8_t)0x2B);
			}
			else {
				writeByte((uint8_t)0x3B);
			}
		}
	}
	else {
		if (regOp2) {
			if (op2AsPtr) {
				writeByte((uint8_t)0x13);
			}
			else {
				writeByte((uint8_t)0x03);
			}
		}
		else {
			if (op2AsPtr) {
				writeByte((uint8_t)0x23);
			}
			else {
				writeByte((uint8_t)0x33);
			}
		}
	}
	codegen_advance();

	switch (codegen_currentToken().type)
	{
	case TT_IDEN: writeDWord((uint32_t)(uint32_t)getIdenOffset(codegen_currentToken().lexeme)); break;
	case TT_REG: {
		int value = 0;
		if (string(codegen_currentToken().lexeme) == "rsp") value = 28;
		else if (string(codegen_currentToken().lexeme) == "rbp") value = 29;
		else if (string(codegen_currentToken().lexeme) == "rf0") value = 30;
		else if (string(codegen_currentToken().lexeme) == "rf1") value = 31;
		else if (string(codegen_currentToken().lexeme) == "rf2") value = 32;
		else if (string(codegen_currentToken().lexeme) == "rf3") value = 33;
		else value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		break;
	}
	case TT_NUMBER: {
		uint64_t value = 0;
		if (codegen_currentToken().lexeme[1] == 'x') {
			value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
			writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().lexeme[1] == 'b') {
			value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
			writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			value = std::stoi(string(codegen_currentToken().lexeme));
			writeDWord((uint32_t)(uint32_t)value);
		}
		break;
	}
	default: break;
	}

	codegen_advance();
	codegen_advance();

	switch (codegen_currentToken().type)
	{
	case TT_CHAR: {
		for (size_t i = 0; codegen_currentToken().lexeme[i] != '\0'; i++)
		{
			writeByte((uint8_t)codegen_currentToken().lexeme[i]);
		}
		break;
	}
	case TT_CURRENT: writeDWord((uint32_t)(uint32_t)c.curByte); break;
	case TT_IDEN: writeDWord((uint32_t)(uint32_t)getIdenOffset(codegen_currentToken().lexeme)); break;
	case TT_LBRACKET:
	case TT_NUMBER: {
		uint64_t value = 0;
		if (codegen_currentToken().lexeme[1] == 'x') {
			value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
			writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().lexeme[1] == 'b') {
			value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
			writeDWord((uint32_t)value);
		}
		else {
			value = std::stoi(string(codegen_currentToken().lexeme));
			writeDWord((uint32_t)value);
		}
		break;
	}
	case TT_SECT_START: writeDWord((uint32_t)getIdenOffset(c.curSect.lexeme)); break;
	case TT_REG: {
		int value = 0;
		if (string(codegen_currentToken().lexeme) == "rsp") value = 28;
		else if (string(codegen_currentToken().lexeme) == "rbp") value = 29;
		else if (string(codegen_currentToken().lexeme) == "rf0") value = 30;
		else if (string(codegen_currentToken().lexeme) == "rf1") value = 31;
		else if (string(codegen_currentToken().lexeme) == "rf2") value = 32;
		else if (string(codegen_currentToken().lexeme) == "rf3") value = 33;
		else value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		break;
	}
	default:
		break;
	}
	codegen_advance();
}

void movw_instruction() {
	bool regOp1 = false;
	bool regOp2 = false;
	bool op2AsPtr = false;

	if (codegen_getToken(3).type == TT_REG) regOp2 = true;
	else if (codegen_getToken(3).type == TT_LBRACKET) {
		op2AsPtr = true;
		if (codegen_getToken(4).type == TT_REG) regOp2 = true;
	}

	if (codegen_getToken(1).type == TT_REG) regOp1 = true;

	if (regOp2) writeByte((uint8_t)0xD0);
	else writeByte((uint8_t)0xC0);

	if (regOp1) {
		if (regOp2) {
			if (op2AsPtr) {
				writeByte((uint8_t)0x1B);
			}
			else {
				writeByte((uint8_t)0x0B);
			}
		}
		else {
			if (op2AsPtr) {
				writeByte((uint8_t)0x2B);
			}
			else {
				writeByte((uint8_t)0x3B);
			}
		}
	}
	else {
		if (regOp2) {
			if (op2AsPtr) {
				writeByte((uint8_t)0x13);
			}
			else {
				writeByte((uint8_t)0x03);
			}
		}
		else {
			if (op2AsPtr) {
				writeByte((uint8_t)0x23);
			}
			else {
				writeByte((uint8_t)0x33);
			}
		}
	}
	codegen_advance();

	switch (codegen_currentToken().type)
	{
	case TT_IDEN: writeWord((uint16_t)getIdenOffset(codegen_currentToken().lexeme)); break;
	case TT_REG: {
		int value = 0;
		if (string(codegen_currentToken().lexeme) == "rsp") value = 28;
		else if (string(codegen_currentToken().lexeme) == "rbp") value = 29;
		else if (string(codegen_currentToken().lexeme) == "rf0") value = 30;
		else if (string(codegen_currentToken().lexeme) == "rf1") value = 31;
		else if (string(codegen_currentToken().lexeme) == "rf2") value = 32;
		else if (string(codegen_currentToken().lexeme) == "rf3") value = 33;
		else value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		break;
	}
	case TT_NUMBER: {
		uint64_t value = 0;
		if (codegen_currentToken().lexeme[1] == 'x') {
			value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
			writeWord((uint16_t)value);
		}
		else if (codegen_currentToken().lexeme[1] == 'b') {
			value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
			writeWord((uint16_t)value);
		}
		else {
			value = std::stoi(string(codegen_currentToken().lexeme));
			writeWord((uint16_t)value);
		}
		break;
	}
	default: break;
	}

	codegen_advance();
	codegen_advance();

	switch (codegen_currentToken().type)
	{
	case TT_CHAR: {
		for (size_t i = 0; codegen_currentToken().lexeme[i] != '\0'; i++)
		{
			writeByte((uint8_t)codegen_currentToken().lexeme[i]);
		}
		break;
	}
	case TT_CURRENT: writeWord((uint16_t)c.curByte); break;
	case TT_IDEN: writeWord((uint16_t)getIdenOffset(codegen_currentToken().lexeme)); break;
	case TT_LBRACKET:
	case TT_NUMBER: {
		uint64_t value = 0;
		if (codegen_currentToken().lexeme[1] == 'x') {
			value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
			writeWord((uint16_t)value);
		}
		else if (codegen_currentToken().lexeme[1] == 'b') {
			value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
			writeWord((uint16_t)value);
		}
		else {
			value = std::stoi(string(codegen_currentToken().lexeme));
			writeWord((uint16_t)value);
		}
		break;
	}
	case TT_SECT_START: writeWord((uint16_t)getIdenOffset(c.curSect.lexeme)); break;
	case TT_REG: {
		int value = 0;
		if (string(codegen_currentToken().lexeme) == "rsp") value = 28;
		else if (string(codegen_currentToken().lexeme) == "rbp") value = 29;
		else if (string(codegen_currentToken().lexeme) == "rf0") value = 30;
		else if (string(codegen_currentToken().lexeme) == "rf1") value = 31;
		else if (string(codegen_currentToken().lexeme) == "rf2") value = 32;
		else if (string(codegen_currentToken().lexeme) == "rf3") value = 33;
		else value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		break;
	}
	default:
		break;
	}
	codegen_advance();
}

void movb_instruction() {
	bool regOp1 = false;
	bool regOp2 = false;
	bool op2AsPtr = false;

	if (codegen_getToken(3).type == TT_REG) regOp2 = true;
	else if (codegen_getToken(3).type == TT_LBRACKET) {
		op2AsPtr = true;
		if (codegen_getToken(4).type == TT_REG) regOp2 = true;
	}

	if (codegen_getToken(1).type == TT_REG) regOp1 = true;

	if (regOp2) writeByte((uint8_t)0xD0);
	else writeByte((uint8_t)0xC0);

	if (regOp1) {
		if (regOp2) {
			if (op2AsPtr) {
				writeByte((uint8_t)0x1B);
			}
			else {
				writeByte((uint8_t)0x0B);
			}
		}
		else {
			if (op2AsPtr) {
				writeByte((uint8_t)0x2B);
			}
			else {
				writeByte((uint8_t)0x3B);
			}
		}
	}
	else {
		if (regOp2) {
			if (op2AsPtr) {
				writeByte((uint8_t)0x13);
			}
			else {
				writeByte((uint8_t)0x03);
			}
		}
		else {
			if (op2AsPtr) {
				writeByte((uint8_t)0x23);
			}
			else {
				writeByte((uint8_t)0x33);
			}
		}
	}
	codegen_advance();

	switch (codegen_currentToken().type)
	{
	case TT_IDEN: writeByte((uint8_t)getIdenOffset(codegen_currentToken().lexeme)); break;
	case TT_REG: {
		int value = 0;
		if (string(codegen_currentToken().lexeme) == "rsp") value = 28;
		else if (string(codegen_currentToken().lexeme) == "rbp") value = 29;
		else if (string(codegen_currentToken().lexeme) == "rf0") value = 30;
		else if (string(codegen_currentToken().lexeme) == "rf1") value = 31;
		else if (string(codegen_currentToken().lexeme) == "rf2") value = 32;
		else if (string(codegen_currentToken().lexeme) == "rf3") value = 33;
		else value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		break;
	}
	case TT_NUMBER: {
		uint64_t value = 0;
		if (codegen_currentToken().lexeme[1] == 'x') {
			value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
			if (c.bitMode == 64) writeByte((uint8_t)value);
		}
		else if (codegen_currentToken().lexeme[1] == 'b') {
			value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
			if (c.bitMode == 64) writeByte((uint8_t)value);
		}
		else {
			value = std::stoi(string(codegen_currentToken().lexeme));
			if (c.bitMode == 64) writeByte((uint8_t)value);
		}
		break;
	}
	default: break;
	}

	codegen_advance();
	codegen_advance();

	switch (codegen_currentToken().type)
	{
	case TT_CHAR: {
		for (size_t i = 0; codegen_currentToken().lexeme[i] != '\0'; i++)
		{
			writeByte((uint8_t)codegen_currentToken().lexeme[i]);
		}
		break;
	}
	case TT_CURRENT: writeByte((uint8_t)c.curByte); break;
	case TT_IDEN: writeByte((uint8_t)getIdenOffset(codegen_currentToken().lexeme)); break;
	case TT_LBRACKET:
	case TT_NUMBER: {
		uint64_t value = 0;
		if (codegen_currentToken().lexeme[1] == 'x') {
			value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
			if (c.bitMode == 64) writeByte((uint8_t)value);
		}
		else if (codegen_currentToken().lexeme[1] == 'b') {
			value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
			if (c.bitMode == 64) writeByte((uint8_t)value);
		}
		else {
			value = std::stoi(string(codegen_currentToken().lexeme));
			if (c.bitMode == 64) writeByte((uint8_t)value);
		}
		break;
	}
	case TT_SECT_START: writeByte((uint8_t)getIdenOffset(c.curSect.lexeme)); break;
	case TT_REG: {
		int value = 0;
		if (string(codegen_currentToken().lexeme) == "rsp") value = 28;
		else if (string(codegen_currentToken().lexeme) == "rbp") value = 29;
		else if (string(codegen_currentToken().lexeme) == "rf0") value = 30;
		else if (string(codegen_currentToken().lexeme) == "rf1") value = 31;
		else if (string(codegen_currentToken().lexeme) == "rf2") value = 32;
		else if (string(codegen_currentToken().lexeme) == "rf3") value = 33;
		else value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		break;
	}
	default:
		break;
	}
	codegen_advance();
}

void rfl_instruction() {
	writeByte((uint8_t)0x49);
	writeByte((uint8_t)0x01);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void push_instruction() {
	writeByte((uint8_t)0x54);
	writeByte((uint8_t)0x01);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void pop_instruction() {
	writeByte((uint8_t)0x55);
	writeByte((uint8_t)0x01);
	codegen_advance();

	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void jne_instruction() {
	uint64_t value = 0;
	bool regOp = false;
	if (codegen_getToken(1).type == TT_REG) regOp = true;
	else regOp = false;

	writeByte((uint8_t)0x40);
	if (regOp) writeByte((uint8_t)0x09);
	else writeByte((uint8_t)0x01);
	codegen_advance();

	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	else {
		if (codegen_currentToken().type == TT_IDEN) {
			value = getIdenOffset(codegen_currentToken().lexeme);
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_SECT_START) {
			value = c.curSect.offset;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_CURRENT) {
			value = c.curByte;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
		}

		codegen_advance();
	}
}

void jc_instruction() {
	uint64_t value = 0;
	bool regOp = false;
	if (codegen_getToken(1).type == TT_REG) regOp = true;
	else regOp = false;

	writeByte((uint8_t)0x41);
	if (regOp) writeByte((uint8_t)0x09);
	else writeByte((uint8_t)0x01);
	codegen_advance();

	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	else {
		if (codegen_currentToken().type == TT_IDEN) {
			value = getIdenOffset(codegen_currentToken().lexeme);
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_SECT_START) {
			value = c.curSect.offset;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_CURRENT) {
			value = c.curByte;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
		}

		codegen_advance();
	}
}

void jl_instruction() {
	uint64_t value = 0;
	bool regOp = false;
	if (codegen_getToken(1).type == TT_REG) regOp = true;
	else regOp = false;

	writeByte((uint8_t)0x42);
	if (regOp) writeByte((uint8_t)0x09);
	else writeByte((uint8_t)0x01);
	codegen_advance();

	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	else {
		if (codegen_currentToken().type == TT_IDEN) {
			value = getIdenOffset(codegen_currentToken().lexeme);
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_SECT_START) {
			value = c.curSect.offset;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_CURRENT) {
			value = c.curByte;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
		}

		codegen_advance();
	}
}

void jle_instruction() {
	uint64_t value = 0;
	bool regOp = false;
	if (codegen_getToken(1).type == TT_REG) regOp = true;
	else regOp = false;

	writeByte((uint8_t)0x43);
	if (regOp) writeByte((uint8_t)0x09);
	else writeByte((uint8_t)0x01);
	codegen_advance();

	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	else {
		if (codegen_currentToken().type == TT_IDEN) {
			value = getIdenOffset(codegen_currentToken().lexeme);
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_SECT_START) {
			value = c.curSect.offset;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_CURRENT) {
			value = c.curByte;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
		}

		codegen_advance();
	}
}

void jg_instruction() {
	uint64_t value = 0;
	bool regOp = false;
	if (codegen_getToken(1).type == TT_REG) regOp = true;
	else regOp = false;

	writeByte((uint8_t)0x44);
	if (regOp) writeByte((uint8_t)0x09);
	else writeByte((uint8_t)0x01);
	codegen_advance();

	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	else {
		if (codegen_currentToken().type == TT_IDEN) {
			value = getIdenOffset(codegen_currentToken().lexeme);
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_SECT_START) {
			value = c.curSect.offset;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_CURRENT) {
			value = c.curByte;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
		}

		codegen_advance();
	}
}

void jge_instruction() {
	uint64_t value = 0;
	bool regOp = false;
	if (codegen_getToken(1).type == TT_REG) regOp = true;
	else regOp = false;

	writeByte((uint8_t)0x45);
	if (regOp) writeByte((uint8_t)0x09);
	else writeByte((uint8_t)0x01);
	codegen_advance();

	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	else {
		if (codegen_currentToken().type == TT_IDEN) {
			value = getIdenOffset(codegen_currentToken().lexeme);
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_SECT_START) {
			value = c.curSect.offset;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_CURRENT) {
			value = c.curByte;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
		}

		codegen_advance();
	}
}

void jo_instruction() {
	uint64_t value = 0;
	bool regOp = false;
	if (codegen_getToken(1).type == TT_REG) regOp = true;
	else regOp = false;

	writeByte((uint8_t)0x46);
	if (regOp) writeByte((uint8_t)0x09);
	else writeByte((uint8_t)0x01);
	codegen_advance();

	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	else {
		if (codegen_currentToken().type == TT_IDEN) {
			value = getIdenOffset(codegen_currentToken().lexeme);
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_SECT_START) {
			value = c.curSect.offset;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_CURRENT) {
			value = c.curByte;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
		}

		codegen_advance();
	}
}

void je_instruction() {
	uint64_t value = 0;
	bool regOp = false;
	if (codegen_getToken(1).type == TT_REG) regOp = true;
	else regOp = false;

	writeByte((uint8_t)0x47);
	if (regOp) writeByte((uint8_t)0x09);
	else writeByte((uint8_t)0x01);
	codegen_advance();

	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	else {
		if (codegen_currentToken().type == TT_IDEN) {
			value = getIdenOffset(codegen_currentToken().lexeme);
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_SECT_START) {
			value = c.curSect.offset;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_CURRENT) {
			value = c.curByte;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
		}

		codegen_advance();
	}
}

void jnc_instruction() {
	uint64_t value = 0;
	bool regOp = false;
	if (codegen_getToken(1).type == TT_REG) regOp = true;
	else regOp = false;

	writeByte((uint8_t)0x48);
	if (regOp) writeByte((uint8_t)0x09);
	else writeByte((uint8_t)0x01);
	codegen_advance();

	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	else {
		if (codegen_currentToken().type == TT_IDEN) {
			value = getIdenOffset(codegen_currentToken().lexeme);
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_SECT_START) {
			value = c.curSect.offset;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_CURRENT) {
			value = c.curByte;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
		}

		codegen_advance();
	}
}

void jmp_instruction() {
	uint64_t value = 0;
	bool regOp = false;
	if (codegen_getToken(1).type == TT_REG) regOp = true;
	else regOp = false;

	writeByte((uint8_t)0x51);
	if (regOp) writeByte((uint8_t)0x09);
	else writeByte((uint8_t)0x01);
	codegen_advance();

	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	else {
		if (codegen_currentToken().type == TT_IDEN) {
			value = getIdenOffset(codegen_currentToken().lexeme);
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_SECT_START) {
			value = c.curSect.offset;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_CURRENT) {
			value = c.curByte;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
		}

		codegen_advance();
	}
}

void call_instruction() {
	uint64_t value = 0;
	bool regOp = false;
	if (codegen_getToken(1).type == TT_REG) regOp = true;
	else regOp = false;

	writeByte((uint8_t)0x52);
	if (regOp) writeByte((uint8_t)0x09);
	else writeByte((uint8_t)0x01);
	codegen_advance();

	if (regOp) {
		value = std::stoi(string(codegen_currentToken().lexeme + 2));
		writeByte((uint8_t)(uint8_t)value);
		codegen_advance();
	}
	else {
		if (codegen_currentToken().type == TT_IDEN) {
			value = getIdenOffset(codegen_currentToken().lexeme);
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_SECT_START) {
			value = c.curSect.offset;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else if (codegen_currentToken().type == TT_CURRENT) {
			value = c.curByte;
			if (c.bitMode == 64) writeQWord(value);
			else writeDWord((uint32_t)(uint32_t)value);
		}
		else {
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
		}

		codegen_advance();
	}
}

void int_instruction() {
	uint64_t value = 0;
	writeByte((uint8_t)0x13);
	writeByte((uint8_t)0x01);
	codegen_advance();

	if (codegen_currentToken().lexeme[1] == 'x') {
		value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
		writeByte((uint8_t)value);
	}
	else if (codegen_currentToken().lexeme[1] == 'b') {
		value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
		writeByte((uint8_t)value);
	}
	else {
		value = std::stoi(string(codegen_currentToken().lexeme));
		writeByte((uint8_t)value);
	}
	codegen_advance();
}

void bits_keyword() {
	codegen_advance();

	uint64_t value = std::stoi(string(codegen_currentToken().lexeme));
	if (value != 64 && value != 32) codegen_error("Bit mode cannot be assigned to a value of " +
		string(codegen_currentToken().lexeme), codegen_currentToken());
	else c.bitMode = (uint8_t)value;

	codegen_advance();
}

void addIden(char* lexeme) {
	if (c.curIden == c.idenSize - 1) {
		Iden* newIdens = new Iden[c.idenSize + 10];
		memcpy(newIdens, c.idens, c.idenSize);
		c.idenSize += 10;
		delete[] c.idens;
		c.idens = newIdens;
	}

	for (size_t i = 0; i < c.idenSize; i++)
	{
		if (c.idens[i].lexeme == lexeme) return;
	}

	c.idens[c.curIden].lexeme = lexeme;
	c.idens[c.curIden++].offset = c.curByte;
}

void section_keyword() {
	codegen_advance();

	while (c.curByte % 16 != 0) writeByte(0);
	addIden(codegen_currentToken().lexeme);

	c.curSect = c.idens[c.curIden - 1];

	codegen_advance();
}

void iden() {
	uint64_t value = 0;
	if (codegen_getToken(1).type == TT_COLON) {
		addIden(codegen_currentToken().lexeme);
		codegen_advance();
		codegen_advance();
	}
	else {
		addIden(codegen_currentToken().lexeme);
		codegen_advance();

		if (codegen_getToken(1).type == TT_NUMBER) {
			codegen_advance();
			if (codegen_currentToken().lexeme[1] == 'x') {
				value = std::stoi(string(codegen_currentToken().lexeme), nullptr, 16);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else if (codegen_currentToken().lexeme[1] == 'b') {
				value = std::stoi(string(codegen_currentToken().lexeme + 2), nullptr, 2);
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			else {
				value = std::stoi(string(codegen_currentToken().lexeme));
				if (c.bitMode == 64) writeQWord(value);
				else writeDWord((uint32_t)(uint32_t)value);
			}
			codegen_advance();
		}
		else {
			codegen_advance();
			for (size_t i = 0; codegen_currentToken().lexeme[i] != '\0'; i++)
			{
				writeByte((uint8_t)codegen_currentToken().lexeme[i]);
			}
			codegen_advance();
		}
	}
}

void in_instruction() {
	writeByte((uint8_t)0x91);
	writeByte((uint8_t)0x03);

	codegen_advance();
	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void out_instruction() {
	writeByte((uint8_t)0xF2);
	writeByte((uint8_t)0x03);

	codegen_advance();
	int value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
	codegen_advance();

	value = std::stoi(string(codegen_currentToken().lexeme + 2));
	writeByte((uint8_t)(uint8_t)value);
	codegen_advance();
}

void ret_instruction() {
	writeByte((uint8_t)0x43);
	writeByte((uint8_t)0x00);
	codegen_advance();
}

void nop_instruction() {
	writeByte((uint8_t)0x00);
	writeByte((uint8_t)0x00);
	codegen_advance();
}

void hlt_instruction() {
	writeByte((uint8_t)0x04);
	writeByte((uint8_t)0x00);
	codegen_advance();
}

void cli_instruction() {
	writeByte((uint8_t)0x05);
	writeByte((uint8_t)0x00);
	codegen_advance();
}

void sti_instruction() {
	writeByte((uint8_t)0x06);
	writeByte((uint8_t)0x00);
	codegen_advance();
}

void pusha_instruction() {
	writeByte((uint8_t)0x46);
	writeByte((uint8_t)0x00);
	codegen_advance();
}

void popa_instruction() {
	writeByte((uint8_t)0x47);
	writeByte((uint8_t)0x00);
	codegen_advance();
}

void iret_instruction() {
	writeByte((uint8_t)0x48);
	writeByte((uint8_t)0x00);
	codegen_advance();
}

void codegen_instruction() {
	switch (codegen_currentToken().type)
	{
	case TT_ADD: add_instruction(); break;
	case TT_SUB: sub_instruction(); break;
	case TT_MUL: mul_instruction(); break;
	case TT_DIV: div_instruction(); break;
	case TT_SHL: shl_instruction(); break;
	case TT_SHR: shr_instruction(); break;
	case TT_AND: and_instruction(); break;
	case TT_CMP: cmp_instruction(); break;
	case TT_OR:  or_instruction();  break;
	case TT_MOVQ: movq_instruction(); break;
	case TT_MOVD: movd_instruction(); break;
	case TT_MOVW: movw_instruction(); break;
	case TT_MOVB: movb_instruction(); break;
	case TT_RFL: rfl_instruction(); break;
	case TT_PUSH: push_instruction(); break;
	case TT_POP: pop_instruction();  break;
	case TT_JNE: jne_instruction(); break;
	case TT_JC: jc_instruction(); break;
	case TT_JL: jl_instruction(); break;
	case TT_JLE: jle_instruction(); break;
	case TT_JG: jg_instruction(); break;
	case TT_JGE: jge_instruction(); break;
	case TT_JO: jo_instruction(); break;
	case TT_JE: je_instruction(); break;
	case TT_JNC: jnc_instruction(); break;
	case TT_JMP: jmp_instruction(); break;
	case TT_CALL: call_instruction(); break;
	case TT_IN: in_instruction(); break;
	case TT_OUT: out_instruction(); break;
	case TT_INT: int_instruction(); break;
	case TT_RET: ret_instruction(); break;
	case TT_NOP: nop_instruction(); break;
	case TT_HLT: hlt_instruction(); break;
	case TT_CLI: cli_instruction(); break;
	case TT_STI: sti_instruction(); break;
	case TT_PUSHA: pusha_instruction(); break;
	case TT_POPA: popa_instruction(); break;
	case TT_IRET: iret_instruction(); break;
	case TT_SECTION: section_keyword(); break;
	case TT_BITS: bits_keyword(); break;
	case TT_IDEN: iden(); break;
	case TT_NEWLINE: codegen_advance(); break;
	case TT_EOF: return;
	default: cout << codegen_currentToken().type << '\n';  codegen_error("Fix your damn codegen", codegen_currentToken()); codegen_advance(); break;
	}
}

CodeStatus codegen() {
	c.output = new uint8_t[50];
	c.idens = new Iden[10];
	c.curSize = 50;

	CodeStatus cs;

	while (codegen_currentToken().type != TT_EOF) {
		codegen_instruction();
	}

	cs.binData = c.output;
	cs.length = c.curByte + 1;
	cs.status = c.state;

	return cs;
}