#pragma once
#include <variant>
#include <string>

namespace Ast{
	struct Identifier{
		int id;
	};

	struct Callable{
		int TODO;
	};

	struct Error{
		std::string err;
	};

	using Value = std::variant<std::string, int, bool, Identifier, Error, Callable>;
}