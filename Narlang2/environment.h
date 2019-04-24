#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "value.h"

namespace Env{
	using Scope = std::unordered_map<std::string, Ast::Value>;

	class Environment{
	public:
		void allocateValue(const std::string& identifier, const Ast::Value& value);
		void setValue(const std::string& identifier, const Ast::Value& value);
		const Ast::Value& getValue(const std::string& identifier);
		void increaseScope();
		void decreaseScope();
	private:
		std::vector<Scope> environment;
	};
}