#include <myloxx/chunk.h>
#include <myloxx/debug.h>
#include <myloxx/VM.h>
#include <fstream>

using namespace myloxx;

void Repl(VM& vm) {
	std::string line;

	while(true) {
		std::cout << "> ";
		if(!getline(std::cin, line)) {
			std::cout << std::endl;
			break;
		}
		vm.Interpret(line);
	}
}

std::string ReadFromFile(const char* filename) {
	std::string line;

	std::ifstream in(filename);

	if(in.is_open()) {

		std::string tmp_line;
		while(getline(in, tmp_line)) {
			line += tmp_line;
		}
	}

	in.close();
	return line;
}

void RunFile(const char* filename, VM& vm) {
	auto source = ReadFromFile(filename);
	InterpretResult result = vm.Interpret(source);

	if(result == InterpretResult::COMPILE_ERROR) exit(65);
	if(result == InterpretResult::RUNTIME_ERROR) exit(70);
}


int main(int argc, const char* argv []) {
	VM vm;

	if(argc == 1)
		Repl(vm);
	else if(argc == 2)
		RunFile(argv[1], vm);
	else {
		fprintf(stderr, "Usage: myloxx [path]\n");
		exit(64);
	}



	return 0;
}