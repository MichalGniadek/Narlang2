#include "pch.h"
#include "environment.h"

using namespace Env;

void Environment::allocateValue(const std::string & identifier, const Ast::Value & value){
	environment.back().emplace(identifier, value);
}

void Environment::setValue(const std::string& identifier, const Ast::Value& value){
	environment.back()[identifier] = value;
}

const Ast::Value& Environment::getValue(const std::string& identifier){
	return environment.back()[identifier];
}

void Environment::increaseScope(){
	environment.emplace_back();
}

void Environment::decreaseScope(){
	environment.pop_back();
}
