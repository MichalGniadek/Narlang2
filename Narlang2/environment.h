#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "value.h"

namespace Env{
	using Scope = std::unordered_map<std::string, Ast::Value>;

	class Environment{
	public:
		void allocateValue(const Ast::Identifier& identifier, const Ast::Value& value);
		void setValue(const Ast::Identifier& identifier, const Ast::Value& value);
		const Ast::Value& getValue(const Ast::Identifier& identifier);
		void increaseScope();
		void decreaseScope();
	private:
		std::vector<Scope> environment;
	};
}