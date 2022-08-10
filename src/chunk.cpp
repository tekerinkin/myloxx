#include <myloxx/chunk.h>

namespace myloxx {

	std::string_view NameOf(op_code code) {
		switch(code) {
		case op_code::RETURN: return "RETURN";
		case op_code::CONSTANT: return "CONSTANT";
		case op_code::NEGATE: return "NEGATE";
		case op_code::ADD: return "ADD";
		case op_code::SUBTRACT: return "SUBTRACT";
		case op_code::DIVIDE : return "DIVIDE";
		case op_code::MULTIPLY: return "MULTIPLY";
		default:
			throw std::invalid_argument("Unexpected op_code: NameOf");
		}
	}

	std::ostream& operator<<(std::ostream& out, op_code code) {
		out << NameOf(code);
		return out;
	}
}
