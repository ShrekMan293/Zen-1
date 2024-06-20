#pragma once
#include "common.h"
#include <algorithm>

enum TokenType {
	// ALU Instructions
	TT_ADD, TT_SUB, TT_MUL, TT_DIV,
	TT_SHL, TT_SHR, TT_AND, TT_OR,
	TT_NOT, TT_NOR, TT_XOR, TT_CMP,

	// Memory Instructions
	TT_MOVQ, TT_MOVD, TT_MOVW, TT_MOVB,
	TT_JMP, TT_CALL, TT_RET, TT_PUSH,
	TT_POP, TT_PUSHA, TT_POPA, TT_IRET,

	// Conditionals
	TT_JNE, TT_JC, TT_JL, TT_JLE,
	TT_JG, TT_JGE, TT_JO, TT_JE,
	TT_JNC, TT_RFL,

	// I/O
	TT_NOP, TT_IN, TT_OUT, TT_INT,
	TT_HLT, TT_CLI, TT_STI,

	// Symbols:
	TT_NEWLINE, TT_COLON, TT_COMMA,
	TT_CURRENT, TT_SECT_START, TT_LBRACKET,
	TT_RBRACKET,

	// Keywords:
	TT_SECTION, TT_BITS, TT_EQU,

	// Literals
	TT_IDEN, TT_NUMBER, TT_CHAR,
	TT_REG,


	TT_UNKNOWN, TT_EOF
};

struct Token {
	TokenType type;
	char* lexeme;
	int line;
	int pos;
};

void initLexer(char* src);
Token* makeTokens();
int ctoi(char c);