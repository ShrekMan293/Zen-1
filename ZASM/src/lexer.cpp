#include "lexer.h"

struct Lexer {
	int line;
	int pos;
	int fpos;
	size_t curTok;
	size_t curSize;
	char* src;
	Token* tokens;
};

Lexer l;

void initLexer(char* src)
{
	l.fpos = 0;
	l.line = 1;
	l.pos = 1;
	l.curTok = 0;
	l.curSize = 50;
	l.src = src;
}

void advance() {
	l.fpos++;
	l.pos++;
}

void advance(int advance) {
	l.fpos += advance;
	l.pos += advance;
}

char current() {
	return l.src[l.fpos];
}

void addToken(Token t) {
	if (l.curTok == l.curSize - 1) {
		Token* newTokens = new Token[l.curSize + 50];
		l.curSize += 50;
		memcpy(newTokens, l.tokens, l.curSize - 50);
		delete[] l.tokens;
		l.tokens = newTokens;
	}

	l.tokens[l.curTok++] = t;
}

void makeToken(TokenType type) {
	Token t;
	
	t.type = type;
	t.line = l.line;
	t.pos = l.pos;
	t.lexeme = nullptr;

	advance();
	addToken(t);
}

bool skipWhitespace() {
	switch (current())
	{
	case '\b':
	case '\t':
	case ' ':
	case '\r': advance(); return true;
	default:
		return false;
	}
}

void match(TokenType type, string match, Token t) {
	std::transform(match.begin(), match.end(), match.begin(), ::tolower);
	string lexeme = string(t.lexeme);
	std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);

	if (lexeme == match) {
		t.type = type;
		addToken(t);
	}
	else {
		t.type = TT_IDEN;
		addToken(t);
	}
}

Token getIden() {
	int length = 0;
	char* buffer = nullptr;

	while (isalpha(l.src[l.fpos + length]) || l.src[l.fpos + length] == '_' || l.src[l.fpos + length] == '.') length++;
	
	if (l.src[l.fpos + 1] == 'x' && isdigit(l.src[l.fpos + length])) {
		length++;
		if (isdigit(l.src[l.fpos + length])) length++;

		while (isalpha(l.src[l.fpos + length]) || l.src[l.fpos + length] == '_' || l.src[l.fpos + length] == '.') length++;
	}

	buffer = new char[length + 1];

	int start = l.fpos;
	while (l.fpos != start + length) {
		buffer[l.fpos - start] = current();
		advance();
	}
	buffer[length] = '\0';


	Token t;
	t.lexeme = buffer;
	t.line = l.line;
	t.pos = l.pos;

	return t;
}

int ctoi(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	else return -1;
}

void makeMOVToken(Token t) {
	switch (tolower(t.lexeme[3]))
	{
	case 'b': t.type = TT_MOVB; addToken(t); break;
	case 'd': t.type = TT_MOVD; addToken(t); break;
	case 'q': t.type = TT_MOVQ; addToken(t); break;
	case 'w': t.type = TT_MOVW; addToken(t); break;
	default: t.type = TT_IDEN; addToken(t); break;
	}
}

void tokenizeIdentifier() {
	Token t = getIden();

	switch (tolower(t.lexeme[0])) {
	case 'a': {
		switch (tolower(t.lexeme[1]))
		{
		case 'd': match(TT_ADD, "add", t); break;
		case 'n': match(TT_AND, "and", t); break;
		default: t.type = TT_IDEN;  addToken(t); break;
		}
		break;
	}
	case 'b': match(TT_BITS, "bits", t); break;
	case 'c': {
		switch (tolower(t.lexeme[1]))
		{
		case 'a': match(TT_CALL, "call", t); break;
		case 'l': match(TT_CLI, "cli", t); break;
		case 'm': match(TT_CMP, "cmp", t); break;
		default: t.type = TT_IDEN;  addToken(t); break;
		}
		break;
	}
	case 'd': match(TT_DIV, "div", t); break;
	case 'e': match(TT_EQU, "equ", t); break;
	case 'h': match(TT_HLT, "hlt", t); break;
	case 'i': {
		switch (tolower(t.lexeme[1]))
		{
		case 'n': {
			string lexeme = string(t.lexeme);
			std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
			if (string(t.lexeme) == "int") {
				t.type = TT_INT;
				addToken(t);
				break;
			}
			else if (string(t.lexeme) == "in") {
				t.type = TT_IN;
				addToken(t);
				break;
			}
			else {
				t.type = TT_IDEN;
				addToken(t);
				break;
			}
		}
		case 'r': match(TT_IRET, "iret", t); break;
		default: t.type = TT_IDEN;  addToken(t); break;
		}
		break;
	}
	case 'j': {
		switch (tolower(t.lexeme[1]))
		{
		case 'c': match(TT_JC, "jc", t); break;
		case 'e': match(TT_JE, "je", t); break;
		case 'g': {
			string lexeme = string(t.lexeme);
			std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
			if (string(t.lexeme) == "jge") {
				t.type = TT_JGE;
				addToken(t);
				break;
			}
			else if (string(t.lexeme) == "jg") {
				t.type = TT_JG;
				addToken(t);
				break;
			}
			else {
				t.type = TT_IDEN;
				addToken(t);
				break;
			}
		}
		case 'l': {
			string lexeme = string(t.lexeme);
			std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
			if (string(t.lexeme) == "jle") {
				t.type = TT_JLE;
				addToken(t);
				break;
			}
			else if (string(t.lexeme) == "jl") {
				t.type = TT_JL;
				addToken(t);
				break;
			}
			else {
				t.type = TT_IDEN;
				addToken(t);
				break;
			}
		}
		case 'm': match(TT_JMP, "jmp", t); break;
		case 'n': {
			string lexeme = string(t.lexeme);
			std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
			if (string(t.lexeme) == "jne") {
				t.type = TT_JNE;
				addToken(t);
				break;
			}
			else if (string(t.lexeme) == "jnc") {
				t.type = TT_JNC;
				addToken(t);
				break;
			}
			else {
				t.type = TT_IDEN;
				addToken(t);
				break;
			}
		}
		case 'o': match(TT_JO, "jo", t); break;
		default: t.type = TT_IDEN; addToken(t); break;
		}
		break;
	}
	case 'm': {
		switch (tolower(t.lexeme[1])) {
		case 'o': makeMOVToken(t); break;
		case 'u': match(TT_MUL, "mul", t); break;
		default: t.type = TT_IDEN; addToken(t); break;
		}
		break;
	}
	case 'n': {
		switch (tolower(t.lexeme[2])) {	// Second letter of all 3 is 'o' so we skip to third char
		case 'p': match(TT_NOP, "nop", t); break;
		case 'r': match(TT_NOR, "nor", t); break;
		case 't': match(TT_NOT, "not", t); break;
		default: t.type = TT_IDEN; addToken(t); break;
		}
		break;
	}
	case 'o': {
		switch (tolower(t.lexeme[1]))
		{
		case 'r': match(TT_OR, "or", t); break;
		case 'u': match(TT_OUT, "out", t); break;
		default: t.type = TT_IDEN; addToken(t); break;
		}
		break;
	}
	case 'p': {
		string lexeme = string(t.lexeme);
		std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
		if (string(t.lexeme) == "pop") {
			t.type = TT_POP;
			addToken(t);
			break;
		}
		else if (string(t.lexeme) == "popa") {
			t.type = TT_POPA;
			addToken(t);
			break;
		}
		else if (string(t.lexeme) == "push") {
			t.type = TT_PUSH;
			addToken(t);
			break;
		}
		else if (string(t.lexeme) == "pusha") {
			t.type = TT_PUSHA;
			addToken(t);
			break;
		}
		else {
			t.type = TT_IDEN;
			addToken(t);
			break;
		}
		break;
	}
	case 'r': {
		switch (tolower(t.lexeme[1]))
		{
		case 'b': match(TT_REG, "rbp", t); break;
		case 'e': match(TT_RET, "ret", t); break;
		case 'f': {
			if (isdigit(t.lexeme[2])) {
				int digit = ctoi(t.lexeme[2]);
				if (digit >= 0 && digit <= 3) {
					t.type = TT_REG;
					addToken(t);
					break;
				}
				else {
					t.type = TT_IDEN;
					addToken(t);
					break;
				}
				break;
			}
			else {
				if (tolower(t.lexeme[2]) == 'l') {
					t.type = TT_RFL;
					addToken(t);
					break;
				}
				t.type = TT_IDEN;
				addToken(t);
				break;
			}
			break;
		}
		case 'p': match(TT_REG, "rpp", t); break;
		case 's': match(TT_REG, "rsp", t); break;
		case 'x': {
			if (isdigit(t.lexeme[2])) {
				int digit = ctoi(t.lexeme[2]);
				if (digit >= 0 && digit <= 27) {
					t.type = TT_REG;
					addToken(t);
					break;
				}
				else {
					t.type = TT_IDEN;
					addToken(t);
					break;
				}
				break;
			}
			else {
				t.type = TT_IDEN;
				addToken(t);
				break;
			}
		}
		default: t.type = TT_IDEN;  addToken(t); break;
		}
		break;
	}
	case 's': {
		switch (tolower(t.lexeme[1])) {
		case 'e': match(TT_SECTION, "section", t); break;
		case 'h': {
			if (tolower(t.lexeme[2]) == 'r') match(TT_SHR, "shr", t);
			else match(TT_SHL, "shl", t);
			break;
		}
		case 'u': match(TT_SUB, "sub", t); break;
		case 't': match(TT_STI, "sti", t); break;
		default: t.type = TT_IDEN; addToken(t); break;
		}
		break;
	}
	case 'x': match(TT_XOR, "xor", t); break;
	default: t.type = TT_IDEN; addToken(t); break;
	}
}

void makeCharToken() {
	int length = 0;

	Token t;
	t.type = TT_CHAR;
	t.line = l.line;
	t.pos = l.pos;
	char* buffer = nullptr;

	while (l.src[length + l.fpos] != '\'') length++;

	buffer = new char[length + 1];

	int start = l.fpos;
	while (l.fpos != start + length) {
		buffer[l.fpos - start] = l.src[l.fpos];
		advance();
	}

	buffer[length] = '\0';
	t.lexeme = buffer;

	addToken(t);
}

void makeNumberToken() {
	int length = 0;

	Token t;
	t.type = TT_NUMBER;
	t.line = l.line;
	t.pos = l.pos;
	char* buffer = nullptr;

	while (isdigit(l.src[l.fpos + length])) length++;

	if (length == 1) {
		if (l.src[l.fpos + length] == 'x' || l.src[l.fpos + length] == 'b') {
			if (l.src[l.fpos] == '0') {
				length++;
				while (isdigit(l.src[l.fpos + length])) length++;
			}
		}
	}

	buffer = new char[length + 1];

	int start = l.fpos;
	while (l.fpos != start + length) {
		buffer[l.fpos - start] = l.src[l.fpos];
		advance();
	}

	buffer[length] = '\0';
	t.lexeme = buffer;

	addToken(t);
}

void tokenizeSymbol() {
	switch (current())
	{
	case '\'': makeCharToken(); break;
	case ':': makeToken(TT_COLON); break;
	case '[': makeToken(TT_LBRACKET); break;
 case ';': while (current() != '\n') advance(); break;
	case ']': makeToken(TT_RBRACKET); break;
	case ',': makeToken(TT_COMMA); break;
	case '$': makeToken(TT_CURRENT); break;
	case '%': makeToken(TT_SECT_START); break;
	case '\n': makeToken(TT_NEWLINE); l.line++; l.pos = 1; break;
	case '\0': makeToken(TT_EOF); break;
	default:
		if (isdigit(current())) makeNumberToken();
		else makeToken(TT_UNKNOWN);
		break;
	}
}

void tokenize() {
	if (isalpha(current()) || current() == '.' || current() == '_') {
		tokenizeIdentifier();
	}
	else tokenizeSymbol();
}

Token* makeTokens()
{
	l.tokens = new Token[50];

	while (l.src[l.fpos] != '\0') {
		while (skipWhitespace());	// Skips until character is no longer whitespace
		tokenize();
	}

	if (l.src[l.fpos] == '\0' && l.tokens[l.curTok - 1].type != TT_EOF) makeToken(TT_EOF);

	return l.tokens;
}