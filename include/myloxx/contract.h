#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

namespace myloxx {
	namespace contract {
		struct Exception : public std::exception {
			Exception(const char* rule, const char* filename, int line) noexcept {
				std::ostringstream oss;
				oss << "Condition '" << rule << " ' violated at " << filename << ":" << line;
				message_ = oss.str();
			}

			virtual ~Exception() {}

		private:
			std::string message_;
		};

		inline constexpr void Assures(bool condition, const char* rule,
										const char* filename, int line) {
			if(!condition)
				throw Exception(rule, filename, line);
		}
	}
}

#define ASSURES(condition) \
	contract::Assures(condition, #condition, __FILE__, __LINE__);

#define ENSURES(condition) ASSURES(condition)
#define EXPECTS(condition) ASSURES(condition)