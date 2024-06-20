#include "common.h"
#include "lexer.h"
#include "syntax.h"
#include "codegen.h"
#include <fstream>

int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "Usage: [Zasm] [srcFile] [binFile]\n";
		return 1;
	}

	std::ifstream srcFile(argv[1], std::ios::in | std::ios::binary);
	if (!srcFile.is_open()) {
		cout << "Failed to open \"" << std::string(argv[1]) << "\"\n";
		return 1;
	}

	srcFile.seekg(0, std::ifstream::_Seekend);
	size_t length = srcFile.tellg();
	srcFile.seekg(0, std::ifstream::_Seekbeg);
	
	char* buffer = new char[length + 1];
	srcFile.read(buffer, length);
	buffer[length] = '\0';

	initLexer(buffer);
	Token* tokens = makeTokens();

	initParser(tokens);
	State ps = parse();

	if (ps.status != PS_CLEAN) {
		if (ps.errorCount == 1) cout << " error.\n";
		else cout << " errors.\n";
	}

	if (ps.status != PS_CLEAN) return ps.status;

	initCodeGen(tokens);
	CodeStatus cs = codegen();

	delete[] buffer;
	for (size_t i = 0; tokens[i].type != TT_EOF; i++)
	{
		delete[] tokens[i].lexeme;
	}
	delete[] tokens;

	std::ofstream binFile(argv[2], std::ios::out | std::ios::binary);
	if (!binFile.is_open()) {
		cout << "Failed to open " << argv[2] << '\n';
		return 1;
	}
	binFile.write((char*)cs.binData, cs.length);

	delete[] cs.binData;

	if (cs.status.status != PS_CLEAN) {
		cout << "Returned with " << cs.status.errorCount + ps.errorCount;

		if (cs.status.errorCount == 1) cout << " error.\n";
		else cout << " errors.\n";
	}
	else cout << "Build succeeded!\n";

	cout << std::flush;

	return cs.status.status;
}