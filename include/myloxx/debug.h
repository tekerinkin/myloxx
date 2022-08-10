#pragma once

#include <myloxx/chunk.h>

namespace myloxx {
	void DisassembleChunk(const ChunkImpl& chunk, const char* name);

	size_t DisassembleInstruction(const ChunkImpl* chunk, size_t offset);
}