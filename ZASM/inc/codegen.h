#pragma once
#include "common.h"
#include "syntax.h"
#include <cctype>

struct CodeStatus {
	uint8_t* binData;
	size_t length;
	State status;
};

void initCodeGen(Token* tokens);
CodeStatus codegen();

struct Iden {
	char* lexeme;
	uint64_t offset;
};