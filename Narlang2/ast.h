#pragma once
#include <string>
#include <memory>
#include "value.h"
#include "environment.h"

namespace Ast{
	class Node{
	public:
		virtual const Value run() const = 0;
	};

	class RootNode{
	public:
		RootNode(Node* node);
		const Error run() const;
	private:
		const std::unique_ptr<const Node> node;
	};

	class Print final : public Node{
	public:
		Print(Node* node);
		const Value run() const;
	private:
		const std::unique_ptr<const Node> node;
	};

	class Literal final : public Node{
	public:
		Literal(Value value);
		const Value run() const;
	private:
		const Value value;
	};

	class BinaryOperator final : public Node{
	public:
		enum Type{Addition, Substraction, Multiplication, Division, Equal, NotEqual, LessEqual, Less, GreaterEqual, Greater,
		And, Or};
		BinaryOperator(const Type t, Node const * const n1, Node const * const n2);
		const Value run() const;
	private:
		const Type type;
		const std::unique_ptr<const Node> n1;
		const std::unique_ptr<const Node> n2;
	};

	class If_n final : public Node{
	public:
		If_n(Node const * const condition, Node const * const body, Node const * const else_body = nullptr);
		const Value run() const;
	private:
		const std::unique_ptr<const Node> condition;
		const std::unique_ptr<const Node> body;
		const std::unique_ptr<const Node> else_body;
	};

	class While_n final : public Node{
	public:
		While_n(Node const * const condition, Node const * const body);
		const Value run() const;
	private:
		const std::unique_ptr<const Node> condition;
		const std::unique_ptr<const Node> body;
	};

	class Block final : public Node{
	public:
		Block(const std::vector<Node*>& nodes);
		const Value run() const;
	private:
		std::vector<std::unique_ptr<Node>> nodes;
	};
}