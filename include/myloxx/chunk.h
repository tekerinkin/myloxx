#pragma once
#include <cstdint>
#include <vector>
#include <type_traits>
#include <string_view>
#include <iostream>
#include <unordered_set>
#include <myloxx/contract.h>
#include <myloxx/value.h>

namespace myloxx {

	enum class op_code : uint8_t {
		RETURN,
		CONSTANT,
		NEGATE,
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE
	};

	template<typename T>
	using IsTSuitOpcode = std::disjunction<std::is_same<uint8_t, T>, std::is_same<op_code, T>>;

	template<typename... Ts>
	using OpcodeTrait = std::conjunction<IsTSuitOpcode<Ts>...>;

	template<typename... Ts>
	constexpr bool kOpcodeTraitV = OpcodeTrait<Ts...>::value;

	std::string_view NameOf(op_code code);
	std::ostream& operator<<(std::ostream& out, op_code code);

	namespace {
		template<template<typename> typename Alloc = Allocator>
		struct ChunkT {

			template<typename T>
			typename std::enable_if_t<kOpcodeTraitV<T>, void>
			Add(T byte, size_t line) {
				bytecode_.push_back(static_cast<uint8_t>(byte));
				lines_.push_back(line);
			}

			size_t GetSize() const {
				return bytecode_.size();
			}

			uint8_t GetInstruction(size_t offset) const {
				return bytecode_[offset];
			}

			template<typename T>
			 typename std::enable_if_t<std::is_convertible_v<T, Value>, size_t>
			AddConstant(op_code op_code, T&& value, size_t line) {
				values_.Write(std::forward<T>(value));
				this->Add(op_code, line);
				if(values_.Count() - 1 > std::numeric_limits<uint8_t>::max()) {
					bytecode_.push_back(std::numeric_limits<uint8_t>::max());
					bytecode_.push_back(values_.Count() - 1 - std::numeric_limits<uint8_t>::max());
				} else {
					bytecode_.push_back(values_.Count() - 1);
					bytecode_.push_back(0);
				}

				lines_.push_back(line);
				lines_.push_back(line);
				return values_.Count() - 1;
			}

			const Value& GetValue(uint8_t constant) const{
				return values_.At(constant);
			}

			size_t GetLine(size_t offset) const {
				return lines_.at(offset);
			}

			uint8_t* CodeBegin() {
				return &bytecode_[0];
			}

		private:
			std::vector<uint8_t, Alloc<uint8_t>> bytecode_; // Store the op_codes and data associated with it
			ValueArray<Alloc> values_;
			std::vector<size_t, Alloc<size_t>> lines_;
		};
	}

	struct ChunkImpl final : public ChunkT<std::allocator>
	{};

} //myloxx
