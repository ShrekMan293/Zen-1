#include "syntax.h"

struct Parser {
	Token* tokens;
	int curToken;
	State state;
};

Parser p;

void initParser(Token* tokens) {
	p.tokens = tokens;
	p.curToken = 0;
	p.state.status = PS_CLEAN;
	p.state.errorCount = 0;
}

void tok_advance() {
	p.curToken++;
}

void tok_advance(int offset) {
	p.curToken += offset;
}

Token getToken(int offset) {
	return p.tokens[p.curToken + offset];
}

Token currentToken() {
	return p.tokens[p.curToken];
}

void error(string message, Token t) {
	cout << "Error: " << message << " at line " << t.line << ", char " << t.pos << ".\n";
	p.state.errorCount++;
	p.state.status = PS_CRITICAL;
}

void expect(TokenType expected, string message, StateEnum state) {
	if (getToken(1).type != expected) {
		if (state == PS_CRITICAL) {
			error(message, getToken(1));
		}
	}

	tok_advance();
}

void expect(TokenType expect1, TokenType expect2, string message, StateEnum state) {
	if (getToken(1).type != expect1 && getToken(1).type != expect2) {
		if (state == PS_CRITICAL) {
			error(message, getToken(1));
		}
	}

	tok_advance();
}

void parseALUInstruction() {
	expect(TT_REG, "Expected register", PS_CRITICAL);
	if (currentToken().lexeme[1] != 'x') error("Invalid register operation", currentToken());

	expect(TT_COMMA, "Expected separator", PS_CRITICAL);
	expect(TT_REG, "Expected register", PS_CRITICAL);
	if (currentToken().lexeme[1] != 'x') error("Invalid register operation", currentToken());

	expect(TT_NEWLINE, TT_EOF, "Expected line break or EOF", PS_CRITICAL);
}

void parseMOVInstruction() {
	if (getToken(1).type != TT_NUMBER && getToken(1).type != TT_REG && getToken(1).type != TT_IDEN) {
		error("Invalid operand", getToken(1));
	}
	tok_advance();

	expect(TT_COMMA, "Expected separator", PS_CRITICAL);
	if (getToken(1).type != TT_NUMBER && getToken(1).type != TT_REG && getToken(1).type != TT_LBRACKET
		&& getToken(1).type != TT_CHAR && getToken(1).type != TT_IDEN && getToken(1).type != TT_CURRENT
		&& getToken(1).type != TT_SECT_START) {
		error("Invalid operand", getToken(1));
	}
	tok_advance();
	if (currentToken().type == TT_LBRACKET) {
		if (getToken(1).type != TT_NUMBER && getToken(1).type != TT_REG) {
			error("Invalid operand within brackets", getToken(1));
		}
		tok_advance();
		expect(TT_RBRACKET, "Expected closing bracket", PS_CRITICAL);
		tok_advance();
	}
	expect(TT_NEWLINE, TT_EOF, "Expected line break or EOF", PS_CRITICAL);
}

void parseJMPInstruction() {
	if (getToken(1).type != TT_REG && getToken(1).type != TT_NUMBER && getToken(1).type != TT_IDEN && getToken(1).type != TT_CURRENT
		&& getToken(1).type != TT_SECT_START) error("Invalid operand", getToken(1));
	tok_advance();
	expect(TT_NEWLINE, TT_EOF, "Expected line break or EOF", PS_CRITICAL);
}

void parseStackInstruction() {
	expect(TT_REG, "Invalid operand", PS_CRITICAL);
	if (currentToken().lexeme[1] != 'x') error("Invalid register operation", currentToken());
	expect(TT_NEWLINE, TT_EOF, "Expected line break or EOF", PS_CRITICAL);
}

void parseIOInstruction() {
	expect(TT_REG, "Invalid operand", PS_CRITICAL);
	expect(TT_COMMA, "Expected separator", PS_CRITICAL);
	expect(TT_REG, "Invalid operand", PS_CRITICAL);
	expect(TT_NEWLINE, TT_EOF, "Expected line break or EOF", PS_CRITICAL);
}

void parseIntOperation() {
	expect(TT_NUMBER, "Expected interrupt value", PS_CRITICAL);
	expect(TT_NEWLINE, TT_EOF, "Expected line break or EOF", PS_CRITICAL);
}

void parseNoOperand() {
	expect(TT_NEWLINE, TT_EOF, "Expected line break or EOF", PS_CRITICAL);
}

void parseSection() {
	expect(TT_IDEN, "Expected section name", PS_CRITICAL);
	expect(TT_NEWLINE, TT_EOF, "Expected line break or EOF", PS_CRITICAL);
}

void parseBits() {
	expect(TT_NUMBER, "Expected bit size", PS_CRITICAL);
	expect(TT_NEWLINE, TT_EOF, "Expected line break or EOF", PS_CRITICAL);
}

void parseIden() {
	if (getToken(1).type != TT_COLON && getToken(1).type != TT_EQU) {
		error("Invalid token following identifier", getToken(1));
	}
	tok_advance();
	if (currentToken().type == TT_EQU) {
		if (getToken(1).type != TT_NUMBER && getToken(1).type != TT_CHAR) {
			error("Invalid identifier assignment", getToken(1));
		}
		tok_advance();
		expect(TT_NEWLINE, TT_EOF, "Expected line break or EOF", PS_CRITICAL);
		return;
	}
	else expect(TT_NEWLINE, TT_EOF, "Expected line break or EOF", PS_CRITICAL);
}

void parseCurrent() {
	switch (currentToken().type)
	{
	case TT_ADD:
	case TT_SUB:
	case TT_MUL:
	case TT_DIV:
	case TT_SHL:
	case TT_SHR:
	case TT_AND:
	case TT_CMP:
	case TT_OR: parseALUInstruction(); break;
	case TT_MOVQ:
	case TT_MOVD:
	case TT_MOVW:
	case TT_MOVB: parseMOVInstruction(); break;
	case TT_RFL:	// Not a stack but same operands
	case TT_PUSH:
	case TT_POP: parseStackInstruction(); break;
	case TT_JNE:
	case TT_JC:
	case TT_JL:
	case TT_JLE:
	case TT_JG:
	case TT_JGE:
	case TT_JO:
	case TT_JE:
	case TT_JNC:
	case TT_JMP:
	case TT_CALL: parseJMPInstruction(); break;
	case TT_IN:
	case TT_OUT: parseIOInstruction(); break;
	case TT_INT: parseIntOperation(); break;
	case TT_RET:
	case TT_NOP:
	case TT_HLT:
	case TT_CLI:
	case TT_STI:
	case TT_PUSHA:
	case TT_POPA:
	case TT_IRET: parseNoOperand(); break;
	case TT_SECTION: parseSection(); break;
	case TT_BITS: parseBits(); break;
	case TT_IDEN: parseIden(); break;
	case TT_NEWLINE: tok_advance(); break;
	case TT_EOF: return;
	case TT_UNKNOWN: error("Unknown token", currentToken()); tok_advance(); break;
	default: error("Unexpected token", currentToken()); tok_advance(); break;
	}
}

State parse() {
	while (currentToken().type != TT_EOF) {
		parseCurrent();
	}

	return p.state;
}