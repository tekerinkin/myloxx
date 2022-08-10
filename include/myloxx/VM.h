#pragma once
#include <myloxx/chunk.h>
#include <myloxx/Compiler.h>
#include <memory>
#include <stack>

namespace myloxx {

	enum class InterpretResult {
		OK,
		COMPILE_ERROR,
		RUNTIME_ERROR
	};


	class VM {
	public:
		VM();
		~VM();
		InterpretResult Run();
		InterpretResult Interpret(ChunkImpl&& chunk);
		InterpretResult Interpret(std::string& source);

	private:
		std::unique_ptr<ChunkImpl> chunk_;
		uint8_t* ip_;
		std::vector<Value> stack_;

		void Push(Value val);
		Value Pop();

		VM(VM&) = delete;
		VM(VM&&) = delete;
	};
}