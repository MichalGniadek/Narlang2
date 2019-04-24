#include "pch.h"
#include "ast.h"

#include <iostream>

using namespace Ast;

RootNode::RootNode(Node * node) : node(node){}

const Error RootNode::run() const{
	auto v = node->run();
	if(auto p = std::get_if<Error>(&v)) return *p;
	return {};
}

Print::Print(Node * node) : node(node){}

const Value Print::run() const{
	Value val = node->run();
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

const Value Literal::run() const{
	return value;
}

BinaryOperator::BinaryOperator(const Type t, Node const * const n1, Node const * const n2) : type(t), n1(n1), n2(n2) {}

const Value BinaryOperator::run() const{
	Value val1 = n1->run();
	Value val2 = n2->run();
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

If_n::If_n(Node const * const condition, Node const * const body, Node const * const else_body) : 
		condition(condition), body(body), else_body(else_body) {}

const Value If_n::run() const{
	Value c = condition->run();
	if(auto p = std::get_if<bool>(&c); p){
		if(*p){
			return body->run();
		} else if(else_body != nullptr){
			return else_body->run();
		} else{
			return false;
		}
	} else{
		return Error{"Ast error: if condition isn't of type bool"};
	}
}

While_n::While_n(Node const * const condition, Node const * const body) : condition(condition), body(body) {}

const Value While_n::run() const{
	Value c = condition->run();
	bool const * p = std::get_if<bool>(&c);
	while((p) && (*p)){
		const Value ret = body->run();
		p = std::get_if<bool>(&condition->run());
		if(!(p)) break;
		if(!(*p)) return ret;
	}
	if(!(p)) return Error{"Ast error: while condition isn't of type bool"};
	return false;
}

Block::Block(const std::vector<Node*>& nodes) : nodes(){
	for(auto p : nodes){
		this->nodes.emplace_back(p);
	}
}

const Value Block::run() const{
	if(nodes.empty()) return false;
	for(size_t i = 0; i < nodes.size() - 1; i++){
		nodes[i]->run();
	}
	return nodes.back()->run();
}