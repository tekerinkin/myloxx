#include <iomanip>
#include <iostream>
#include <myloxx/debug.h>

namespace myloxx {

	size_t SimpleInstruction(std::string_view name, size_t offset) {
		std::cout << name << '\n';
		return offset+1;
	}

	size_t ConstantInstruction(std::string_view name, const ChunkImpl* chunk, size_t offset) {
		uint8_t constant = chunk->GetInstruction(offset + 1) + chunk->GetInstruction(offset+2);
		std::cout << std::setfill(' ') << std::left << std::setw(16) << name << ' ';
		std::cout << std::setw(4) << static_cast<unsigned>(constant) << " '";
		std::cout << chunk->GetValue(constant) << "'\n";
		return offset+3;
	}


	size_t DisassembleInstruction(const ChunkImpl* chunk, size_t offset) {
		std::cout << std::setfill('0') << std::right << std::setw(4) << offset << ' ';
		auto chunk_size = chunk->GetSize();

		if(offset >= chunk_size)
			return offset - 1;

		if(offset > 0 && chunk->GetLine(offset) == chunk->GetLine(offset - 1))
			std::cout << "   | ";
		else
			std::cout << std::setfill(' ') << std::right << std::setw(4) << chunk->GetLine(offset) << " ";

		auto instruction = static_cast<op_code>(chunk->GetInstruction(offset));
		switch(instruction) {
		case op_code::CONSTANT:
			return ConstantInstruction(NameOf(instruction), chunk, offset);
		case op_code::RETURN:
		case op_code::NEGATE:
		case op_code::ADD:
		case op_code::SUBTRACT:
		case op_code::DIVIDE:
		case op_code::MULTIPLY:
			return SimpleInstruction(NameOf(instruction), offset);
		default:
			std::cout << "Unknown op_code " << instruction;
			return offset + 1;
		}
	}

	void DisassembleChunk(const ChunkImpl& chunk, const char* name)
	{
		printf("== %s ==\n", name);

		auto chunk_size = chunk.GetSize();

		for (size_t offset = 0; offset < chunk_size;)
		{
			offset = DisassembleInstruction(&chunk, offset);
		}
	}

}