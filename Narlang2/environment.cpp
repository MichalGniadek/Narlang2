#include "pch.h"
#include "environment.h"

using namespace Env;

void Environment::allocateValue(const Ast::Identifier& identifier, const Ast::Value & value){
	environment.back().emplace(identifier.id, value);
}

void Environment::setValue(const Ast::Identifier& identifier, const Ast::Value& value){
	environment.back()[identifier.id] = value;
}

const Ast::Value& Environment::getValue(const Ast::Identifier& identifier){
	return environment.back()[identifier.id];
}

void Environment::increaseScope(){
	environment.emplace_back();
}

void Environment::decreaseScope(){
	environment.pop_back();
}
