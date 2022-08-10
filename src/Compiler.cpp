//
// Created by Дима Шестаков on 08.08.2022.
//

#include "myloxx/Compiler.h"

namespace myloxx {

	void Compiler::Compile(std::string source)
	{

		Scanner scanner(source);
		int line = -1;

		while(true) {
			Token token = scanner.ScanToken();
			if(token.line_ != line) {
				printf("%4d ", token.line_);
				line = token.line_;
			} else
				printf("   | ");

			 std::cout << token.NameOfType() << " " << std::setw(token.length_) << "'" << token.start_ << "'\n";

			if(token.type_ == TokenType::TOKEN_EOF) break;
		}
	}
}
