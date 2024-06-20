#pragma once
#include "common.h"
#include "lexer.h"

enum StateEnum {
	PS_CLEAN = 0b00,
	PS_CRITICAL = 0b11
};

struct State {
	StateEnum status;
	int errorCount;
};

void initParser(Token* tokens);
State parse();