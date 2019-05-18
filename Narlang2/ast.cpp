#include "pch.h"
#include "ast.h"

#include <iostream>

using namespace Ast;

RootNode::RootNode(Node * node) : node(node){}

const Error RootNode::run(Env::Environment& env) const{
	auto v = node->run(env);
	if(auto p = std::get_if<Error>(&v)) return *p;
	return {};
}

Print::Print(Node * node) : node(node){}

const Value Print::run(Env::Environment& env) const{
	Value val = node->run(env);
	if(auto p = std::get_if<std::string>(&val)){
		std::cout << *p;
	} else if(auto p = std::get_if<int>(&val)){
		std::cout << *p;
	} else if(auto p = std::get_if<bool>(&val)){
		std::cout << ((*p)?"true":"false");
	} else if(auto p = std::get_if<Identifier>(&val)){
		std::cout << p->id;
	}
	return val;
}

Literal::Literal(Value value) : value(value){}

const Value Literal::run(Env::Environment& env) const{
	return value;
}

BinaryOperator::BinaryOperator(const Type t, Node const * const n1, Node const * const n2) : type(t), n1(n1), n2(n2) {}

const Value BinaryOperator::run(Env::Environment& env) const{
	auto getUnderlyingValue = [&env](const std::unique_ptr<const Node>& n){
		Value val = n->run(env);
		if(auto p = std::get_if<Identifier>(&val); p != nullptr){
			val = env.getValue(*p);
		}
		return val;
	};

	Value val1 = getUnderlyingValue(n1);
	Value val2 = getUnderlyingValue(n2);

	if(auto p = std::get_if<Identifier>(&val2); p != nullptr){
		val2 = env.getValue(*p);
	}
	switch(type){
	case Addition:
		if(auto[p1, p2] = std::make_pair(std::get_if<int>(&val1), std::get_if<int>(&val2)); p1 && p2){
			return (*p1) + (*p2);
		}else if(auto[p1, p2] = std::make_pair(std::get_if<std::string>(&val1), std::get_if<std::string>(&val2)); p1 && p2){
			return (*p1) + (*p2);
		} else{
			return Error{"Ast error: Addition not specified for given value\n"};
		}
		break;
	case Substraction:
		if(auto[p1, p2] = std::make_pair(std::get_if<int>(&val1), std::get_if<int>(&val2)); p1 && p2){
			return (*p1) - (*p2);
		} else{
			return Error{"Ast error: Substraction not specified for given value\n"};
		}
		break;
	case Division:
		if(auto[p1, p2] = std::make_pair(std::get_if<int>(&val1), std::get_if<int>(&val2)); p1 && p2){
			return (*p1) / (*p2);
		} else{
			return Error{"Ast error: Division not specified for given value\n"};
		}
		break;
	case Multiplication:
		if(auto[p1, p2] = std::make_pair(std::get_if<int>(&val1), std::get_if<int>(&val2)); p1 && p2){
			return (*p1) * (*p2);
		} else{
			return Error{"Ast error: Multiplication not specified for given value\n"};
		}
		break;
	case And:
		if(auto[p1, p2] = std::make_pair(std::get_if<bool>(&val1), std::get_if<bool>(&val2)); p1 && p2){
			return (*p1) && (*p2);
		} else{
			return Error{"Ast error: And not specified for given value\n"};
		}
		break;
	case Or:
		if(auto[p1, p2] = std::make_pair(std::get_if<bool>(&val1), std::get_if<bool>(&val2)); p1 && p2){
			return (*p1) || (*p2);
		} else{
			return Error{"Ast error: Or not specified for given value\n"};
		}
		break;
	case Equal:
		if(auto[p1, p2] = std::make_pair(std::get_if<int>(&val1), std::get_if<int>(&val2)); p1 && p2){
			return (*p1) == (*p2);
		} else if(auto[p1, p2] = std::make_pair(std::get_if<bool>(&val1), std::get_if<bool>(&val2)); p1 && p2){
			return (*p1) == (*p2);
		} else if(auto[p1, p2] = std::make_pair(std::get_if<std::string>(&val1), std::get_if<std::string>(&val2)); p1 && p2){
			return (*p1) == (*p2);
		} else {
			return Error{"Ast error: Equal not specified for given value\n"};
		}
		break;
	case NotEqual:
		if(auto[p1, p2] = std::make_pair(std::get_if<int>(&val1), std::get_if<int>(&val2)); p1 && p2){
			return (*p1) != (*p2);
		} else if(auto[p1, p2] = std::make_pair(std::get_if<bool>(&val1), std::get_if<bool>(&val2)); p1 && p2){
			return (*p1) != (*p2);
		} else if(auto[p1, p2] = std::make_pair(std::get_if<std::string>(&val1), std::get_if<std::string>(&val2)); p1 && p2){
			return (*p1) != (*p2);
		} else{
			return Error{"Ast error: NotEqual not specified for given value\n"};
		}
		break;
	case Less:
		if(auto[p1, p2] = std::make_pair(std::get_if<int>(&val1), std::get_if<int>(&val2)); p1 && p2){
			return (*p1) < (*p2);
		} else{
			return Error{"Ast error: Less not specified for given value\n"};
		}
		break;
	case LessEqual:
		if(auto[p1, p2] = std::make_pair(std::get_if<int>(&val1), std::get_if<int>(&val2)); p1 && p2){
			return (*p1) <= (*p2);
		} else{
			return Error{"Ast error: LessEqual not specified for given value\n"};
		}
		break;
	case Greater:
		if(auto[p1, p2] = std::make_pair(std::get_if<int>(&val1), std::get_if<int>(&val2)); p1 && p2){
			return (*p1) > (*p2);
		} else{
			return Error{"Ast error: Greater not specified for given value\n"};
		}
		break;
	case GreaterEqual:
		if(auto[p1, p2] = std::make_pair(std::get_if<int>(&val1), std::get_if<int>(&val2)); p1 && p2){
			return (*p1) >= (*p2);
		} else{
			return Error{"Ast error: GreaterEqual not specified for given value\n"};
		}
		break;
	}
}

IfNode::IfNode(Node const * const condition, Node const * const body, Node const * const else_body) :
		condition(condition), body(body), else_body(else_body) {}

const Value IfNode::run(Env::Environment& env) const{
	Value c = condition->run(env);
	if(auto p = std::get_if<bool>(&c); p){
		if(*p){
			return body->run(env);
		} else if(else_body != nullptr){
			return else_body->run(env);
		} else{
			return Ast::Null();
		}
	} else{
		return Error{"Ast error: if condition isn't of type bool"};
	}
}

WhileNode::WhileNode(Node const * const condition, Node const * const body) : condition(condition), body(body) {}

const Value WhileNode::run(Env::Environment& env) const{
	Value c = condition->run(env);
	bool const * p = std::get_if<bool>(&c);
	while((p) && (*p)){
		const Value ret = body->run(env);
		p = std::get_if<bool>(&condition->run(env));
		if(!(p)) break;
		if(!(*p)) return ret;
	}
	if(!(p)) return Error{"Ast error: while condition isn't of type bool"};
	return Ast::Null();
}

Block::Block(const std::vector<Node*>& nodes) : nodes(){
	for(auto p : nodes){
		this->nodes.emplace_back(p);
	}
}

const Value Block::run(Env::Environment& env) const{
	env.increaseScope();
	if(nodes.empty()) return Ast::Null();
	for(size_t i = 0; i < nodes.size() - 1; i++){
		nodes[i]->run(env);
	}
	Value r = nodes.back()->run(env);
	env.decreaseScope();
	return r;
}

Ast::Var::Var(const std::string& id) : identifier(id) {}

const Value Ast::Var::run(Env::Environment& env) const{
	env.allocateValue(identifier, Ast::Null());
	return Ast::Identifier{identifier};
}

IdentifierNode::IdentifierNode(const std::string& id) : id(id) {}

const Value Ast::IdentifierNode::run(Env::Environment & env) const{
	return Identifier(id);
}

Ast::Assign::Assign(Node const * const lnode, Node const * const rnode) : lnode(lnode), rnode(rnode) {}

const Value Ast::Assign::run(Env::Environment & env) const{
	Identifier id = *std::get_if<Identifier>(&lnode->run(env));
	const Value v = rnode->run(env);
	env.setValue(id, v);
	return v;
}

Ast::DEBUG_GetValue::DEBUG_GetValue(Node const * const node) : node(node) {}

const Value Ast::DEBUG_GetValue::run(Env::Environment & env) const{
	return env.getValue(*std::get_if<Identifier>(&node->run(env)));
}
