#include "myloxx/VM.h"
#include <myloxx/debug.h>

namespace myloxx {
	InterpretResult VM::Interpret(ChunkImpl&& chunk)
	{
		chunk_ = std::make_unique<ChunkImpl>(chunk);
		ip_ = chunk_->CodeBegin();
		return Run();
	}

	InterpretResult VM::Interpret(std::string& source)
	{
		Compiler compiler;
		compiler.Compile(source);
		return InterpretResult::OK;
	}

	InterpretResult VM::Run()
	{
		#define READ_BYTE() (*ip_++)
		#define READ_CONSTANT() (chunk_->GetInstruction(ip_ - chunk_->CodeBegin()) + chunk_->GetInstruction(ip_ - chunk_->CodeBegin()+1))

		#define BINARY_OP(op)\
		do{\
				if(!stack_.back().IsNumber() || !stack_[stack_.size() - 2].IsNumber()){\
					std::cout << "Operands must be numbers.";\
					return InterpretResult::RUNTIME_ERROR;\
				}\
				auto b = Pop().As<double>();\
				auto a = Pop().As<double>();\
				Push(Value(a op b));\
		} while (false);

		while(true) {
			#ifdef DEBUG_TRACE_EXECUTION
			printf("          ");
			for(auto it = stack_.rbegin(); it != stack_.rend(); ++it)
				std::cout << "[ " << *it << " ]";
			printf("\n");

			DisassembleInstruction(chunk_.get(), (size_t)(ip_ - chunk_->CodeBegin()));
			#endif

			auto instruction = static_cast<op_code>(READ_BYTE());

			switch(instruction) {
			case op_code::CONSTANT: {
				auto constant_idx = READ_CONSTANT();
				Push(chunk_->GetValue(constant_idx));
				ip_+=2;
				break;
			}
			case op_code::NEGATE: {
				auto val = Pop();
				if(!val.IsNumber()) {
					std::cout << "Operand must be a number\n";
					return InterpretResult::RUNTIME_ERROR;
				}
				Push(Value(-val.As<double>()));
				break;
			}

			case op_code::ADD:
				BINARY_OP(+)
				break;
			case op_code::SUBTRACT:
				BINARY_OP(-)
				break;
			case op_code::MULTIPLY:
				BINARY_OP(*)
				break;
			case op_code::DIVIDE:
				BINARY_OP(/)
				break;

			case op_code::RETURN: {
				std::cout << Pop() << "\n";
				return InterpretResult::OK;
			}
			}
		}

		#undef READ_BYTE
		#undef READ_CONSTANT
		#undef BINARY_OP
	}

	VM::VM()  : ip_(nullptr)
	{}

	VM::~VM()
	{
		ip_ = nullptr;
	}

	void VM::Push(Value val) {
		stack_.push_back(val);
	}

	Value VM::Pop() {
		auto tmp = stack_.back();
		stack_.pop_back();
		return tmp;
	}
}
