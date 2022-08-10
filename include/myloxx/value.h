#pragma once

#include <string>
#include <utility>
#include <vector>
#include <variant>

#include <myloxx/contract.h>

namespace myloxx {

	struct Obj;

	template<typename T>
	struct Allocator;

	#ifdef NAN_BOXING

	constexpr uint64_t SIGN_BIT = 0x8000000000000000;
	constexpr uint64_t QNAN = 0x7ffc000000000000;

	constexpr uint64_t TAG_NIL = 1;
	constexpr uint64_t TAG_FALSE = 2;
	constexpr uint64_t TAG_TRUE = 3;

	constexpr uint64_t NIL_VAL = (QNAN | TAG_NIL);
	constexpr uint64_t FALSE_VAL = (QNAN | TAG_FALSE);
	constexpr uint64_t TRUE_VAL = (QNAN | TAG_TRUE);

	#endif // NAN_BOXING

	struct Value {
	#ifdef NAN_BOXING

		uint64_t value;

		union DoubleUnion {
			uint64_t bits_;
			double num_;
		};

		constexpr explicit Value(bool b) noexcept : value(b ? TRUE_VAL : FALSE_VAL) {}

		constexpr Value() noexcept : value(QNAN | TAG_NIL) {}
		explicit Value (double num) noexcept : value(reinterpret_cast<uint64_t&>(num)) {}
		explicit Value(Obj* obj) noexcept : value(SIGN_BIT | QNAN | static_cast<uint64_t>(reinterpret_cast<uintptr_t>(obj))) {}

		[[nodiscard]] constexpr bool IsBool() const noexcept {
			return (value & FALSE_VAL) == FALSE_VAL;
		}

		[[nodiscard]] constexpr bool IsNil() const noexcept {
			return value == (QNAN | TAG_NIL);
		}

		[[nodiscard]] constexpr bool IsNumber() const noexcept {
			return (value & QNAN) != QNAN;
		}

		[[nodiscard]] constexpr bool IsObj() const noexcept {
			return (value & (QNAN | SIGN_BIT)) == (QNAN | SIGN_BIT);
		}

		template<typename T>
		[[nodiscard]] constexpr T As() const {
			if constexpr (std::is_same_v<T, bool>) {
				return value == TRUE_VAL;
			} else if constexpr (std::is_same_v<T, double>) {
				return reinterpret_cast<const double&>(value);
			} else if constexpr (std::is_same_v<T, Obj*>) {
				return reinterpret_cast<Obj*>(static_cast<uintptr_t>(value & ~(SIGN_BIT | QNAN)));
			}
		}
	#else
		using VarT = std::variant<bool, std::monostate, double, Obj*>;

		VarT value_;

		constexpr explicit Value(bool value) noexcept : value_(value) {}
		constexpr Value() noexcept : value_(std::monostate()) {}
		constexpr explicit Value(double value) : value_(value) {}
		constexpr explicit Value(Obj* obj) noexcept : value_(obj) {}

		[[nodiscard]] constexpr bool IsBool() const noexcept {
			return std::holds_alternative<bool>(value_);
		}

		[[nodiscard]] constexpr bool IsNil() const noexcept {
			return std::holds_alternative<std::monostate>(value_);
		}

		[[nodiscard]] constexpr bool IsNumber() const noexcept {
			return std::holds_alternative<double>(value_);
		}

		[[nodiscard]] constexpr bool IsObj() const noexcept {
			return std::holds_alternative<Obj*>(value_);
		}

		template<typename T>
		[[nodiscard]] constexpr T As() const {
			return std::get<T>(value_);
		}

	#endif //NAN_BOXING
	};

	std::ostream& operator<<(std::ostream& out, const Value& value);

	template<template<typename>typename Alloc = Allocator>
	struct ValueArray {

		std::vector<Value, Alloc<Value>> values_;

		[[nodiscard]] constexpr uint8_t Count() const noexcept {
			return values_.size();
		}

		template<typename T>
		typename std::enable_if_t<std::is_convertible_v<T, Value>, void>
		    Write(T&& value) {
			values_.emplace_back(std::forward<T>(value));
		}

		const Value& At(uint8_t constant) const{
			return values_.at(constant);
		}
	};
}