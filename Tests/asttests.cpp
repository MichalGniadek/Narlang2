#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Narlang2/ast.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std;

auto check = [](Ast::BinaryOperator::Type t, auto a1, auto a2, bool expected, Env::Environment e){
	Ast::BinaryOperator div(t, new Ast::Literal(a1), new Ast::Literal(a2));
	Assert::IsTrue(*get_if<bool>(&div.run(e)) == expected);
};

TEST_CLASS(AstTests){
public:
	TEST_METHOD(IntLiteral){
		Env::Environment e;
		Ast::Literal node_int(3);
		Assert::IsTrue(*get_if<int>(&node_int.run(e)) == 3);
	}

	TEST_METHOD(BoolLiteral){
		Env::Environment e;
		Ast::Literal node_bool(false);
		Assert::IsTrue(*get_if<bool>(&node_bool.run(e)) == false);

		Ast::Literal node_bool2(true);
		Assert::IsTrue(*get_if<bool>(&node_bool2.run(e)) == true);
	}

	TEST_METHOD(StringLiteral){
		Env::Environment e;
		Ast::Literal node_string(string("asd"));
		Assert::IsTrue(*get_if<std::string>(&node_string.run(e)) == string("asd"));
	}

	TEST_METHOD(Artimetics){
		{
			Env::Environment e;
			Ast::BinaryOperator add_int(Ast::BinaryOperator::Addition, new Ast::Literal(3), new Ast::Literal(5));
			Assert::IsTrue(*get_if<int>(&add_int.run(e)) == 8);
		}
		{
			Env::Environment e;
			Ast::BinaryOperator sub(Ast::BinaryOperator::Substraction, new Ast::Literal(3), new Ast::Literal(5));
			Assert::IsTrue(*get_if<int>(&sub.run(e)) == -2);
		}
		{
			Env::Environment e;
			Ast::BinaryOperator mul(Ast::BinaryOperator::Multiplication, new Ast::Literal(3), new Ast::Literal(5));
			Assert::IsTrue(*get_if<int>(&mul.run(e)) == 15);
		}
		{
			Env::Environment e;
			Ast::BinaryOperator div(Ast::BinaryOperator::Division, new Ast::Literal(6), new Ast::Literal(2));
			Assert::IsTrue(*get_if<int>(&div.run(e)) == 3);
		}
	}

	TEST_METHOD(StringConcatenation){
		Env::Environment e;
		Ast::BinaryOperator add_str(Ast::BinaryOperator::Addition, new Ast::Literal(string("asd")), new Ast::Literal(string("jkl")));
		Assert::IsTrue(*get_if<string>(&add_str.run(e)) == string("asdjkl"));
	}

	TEST_METHOD(LogicOperators){
		Env::Environment e;

		check(Ast::BinaryOperator::Or, false, false, false, e);
		check(Ast::BinaryOperator::Or, true, false, true, e);
		check(Ast::BinaryOperator::Or, false, true, true, e);
		check(Ast::BinaryOperator::Or, true, true, true, e);

		check(Ast::BinaryOperator::And, false, false, false, e);
		check(Ast::BinaryOperator::And, true, false, false, e);
		check(Ast::BinaryOperator::And, false, true, false, e);
		check(Ast::BinaryOperator::And, true, true, true, e);
	}

	TEST_METHOD(ComparisonOperators){
		Env::Environment e;

		check(Ast::BinaryOperator::Equal, 1, 1, true, e);
		check(Ast::BinaryOperator::Equal, 1, 2, false, e);

		check(Ast::BinaryOperator::Equal, string("asd"), string("asd"), true, e);
		check(Ast::BinaryOperator::Equal, string("asd"), string("aTd"), false, e);

		check(Ast::BinaryOperator::NotEqual, 1, 1, false, e);
		check(Ast::BinaryOperator::NotEqual, 1, 2, true, e);

		check(Ast::BinaryOperator::Less, 1, 2, true, e);
		check(Ast::BinaryOperator::Less, 3, 2, false, e);
		check(Ast::BinaryOperator::Less, 2, 2, false, e);

		check(Ast::BinaryOperator::LessEqual, 1, 2, true, e);
		check(Ast::BinaryOperator::LessEqual, 3, 2, false, e);
		check(Ast::BinaryOperator::LessEqual, 2, 2, true, e);

		check(Ast::BinaryOperator::Greater, 1, 2, false, e);
		check(Ast::BinaryOperator::Greater, 3, 2, true, e);
		check(Ast::BinaryOperator::Greater, 2, 2, false, e);

		check(Ast::BinaryOperator::GreaterEqual, 1, 2, false, e);
		check(Ast::BinaryOperator::GreaterEqual, 3, 2, true, e);
		check(Ast::BinaryOperator::GreaterEqual, 2, 2, true, e);
	}

	TEST_METHOD(If_n){
		{
			Env::Environment e;
			Ast::IfNode n(new Ast::Literal(true), new Ast::Literal(3), new Ast::Literal(5));
			Ast::Value v = n.run(e);
			auto p = std::get_if<int>(&v);
			Assert::IsFalse(p == nullptr);
			Assert::IsTrue((*p) == 3);
		}
		{
			Env::Environment e;
			Ast::IfNode n(new Ast::Literal(false), new Ast::Literal(3), new Ast::Literal(5));
			Ast::Value v = n.run(e);
			auto p = std::get_if<int>(&v);
			Assert::IsFalse(p == nullptr);
			Assert::IsTrue((*p) == 5);
		}
		{
			Env::Environment e;
			Ast::IfNode n(new Ast::Literal(true), new Ast::Literal(3));
			Ast::Value v = n.run(e);
			auto p = std::get_if<int>(&v);
			Assert::IsFalse(p == nullptr);
			Assert::IsTrue((*p) == 3);
		}
		{
			Env::Environment e;
			Ast::IfNode n(new Ast::Literal(false), new Ast::Literal(3));
			Ast::Value v = n.run(e);
			auto p = std::get_if<Ast::Null>(&v);
			Assert::IsFalse(p == nullptr);
		}
	}

	//Can't be tested without variables so will be tested in Ast-Environment integration tests
	//TEST_METHOD(While_n){}

	TEST_METHOD(Block){
		Env::Environment e;
		Ast::Node * l1 = new Ast::Literal(1);
		Ast::Node * l2 = new Ast::Literal(2);
		Ast::Node * l3 = new Ast::Literal(3);
		Ast::Node * l4 = new Ast::Literal(4);

		Ast::Block b({l1, l2, l3, l4});
		auto p = std::get_if<int>(&b.run(e));
		Assert::IsFalse(p == nullptr);
		Assert::IsTrue((*p) == 4);
	}
};
