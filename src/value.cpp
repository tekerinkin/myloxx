#include <myloxx/value.h>
#include <iostream>

namespace myloxx {
	std::ostream& operator<<(std::ostream& out, const Value& value){
		#ifdef NAN_BOXING
			if(value.IsBool())
				out << std::boolalpha << value.As<bool>() << std::noboolalpha;
			else if(value.IsNil())
				out << "nil";
			else if(value.IsNumber())
				out << value.As<double>();
		#else
			std::visit([&out](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;
				if constexpr(std::is_same_v<T, bool>)
					out << std::boolalpha << arg << std::noboolalpha;
				else if constexpr(std::is_same_v<T, std::monostate>)
					out << "nil";
				else if constexpr(std::is_same_v<T, double>)
					out << arg;
			}, value.value_);
		#endif
			return out;
	}
}