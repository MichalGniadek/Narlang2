#pragma once
#include <variant>
#include <string>

namespace Ast{
	struct Identifier{
		Identifier(const std::string s) : id(s) {}
		std::string id;
	};

	struct Callable{
		int TODO;
	};

	struct Error{
		std::string err;
	};

	struct Null{

	};

	using Value = std::variant<std::string, int, bool, Identifier, Error, Callable, Null>;
}