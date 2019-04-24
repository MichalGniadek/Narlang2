#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Narlang2/ast.h"
#include "../Narlang2/ast.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std;

TEST_CLASS(AstTests){
public:
	TEST_METHOD(IntLiteral){
		Ast::Literal node_int(3);
		Assert::IsTrue(*get_if<int>(&node_int.run()) == 3);
	}

	TEST_METHOD(BoolLiteral){
		Ast::Literal node_bool(false);
		Assert::IsTrue(*get_if<bool>(&node_bool.run()) == false);

		Ast::Literal node_bool2(true);
		Assert::IsTrue(*get_if<bool>(&node_bool2.run()) == true);
	}

	TEST_METHOD(StringLiteral){
		Ast::Literal node_string(string("asd"));
		Assert::IsTrue(*get_if<std::string>(&node_string.run()) == string("asd"));
	}

	TEST_METHOD(Artimetics){
		{
			Ast::BinaryOperator add_int(Ast::BinaryOperator::Addition, new Ast::Literal(3), new Ast::Literal(5));
			Assert::IsTrue(*get_if<int>(&add_int.run()) == 8);
		}
		{
			Ast::BinaryOperator sub(Ast::BinaryOperator::Substraction, new Ast::Literal(3), new Ast::Literal(5));
			Assert::IsTrue(*get_if<int>(&sub.run()) == -2);
		}
		{
			Ast::BinaryOperator mul(Ast::BinaryOperator::Multiplication, new Ast::Literal(3), new Ast::Literal(5));
			Assert::IsTrue(*get_if<int>(&mul.run()) == 15);
		}
		{
			Ast::BinaryOperator div(Ast::BinaryOperator::Division, new Ast::Literal(6), new Ast::Literal(2));
			Assert::IsTrue(*get_if<int>(&div.run()) == 3);
		}
	}

	TEST_METHOD(StringConcatenation){
		Ast::BinaryOperator add_str(Ast::BinaryOperator::Addition, new Ast::Literal(string("asd")), new Ast::Literal(string("jkl")));
		Assert::IsTrue(*get_if<string>(&add_str.run()) == string("asdjkl"));
	}

	TEST_METHOD(LogicOperators){
		auto check = [](Ast::BinaryOperator::Type t, bool b1, bool b2, bool expected){
			Ast::BinaryOperator div(t, new Ast::Literal(b1), new Ast::Literal(b2));
			Assert::IsTrue(*get_if<bool>(&div.run()) == expected);
		};

		check(Ast::BinaryOperator::Or, false, false, false);
		check(Ast::BinaryOperator::Or, true, false, true);
		check(Ast::BinaryOperator::Or, false, true, true);
		check(Ast::BinaryOperator::Or, true, true, true);

		check(Ast::BinaryOperator::And, false, false, false);
		check(Ast::BinaryOperator::And, true, false, false);
		check(Ast::BinaryOperator::And, false, true, false);
		check(Ast::BinaryOperator::And, true, true, true);
	}

	TEST_METHOD(ComparisonOperators){
		auto check = [](Ast::BinaryOperator::Type t, auto a1, auto a2, bool expected){
			Ast::BinaryOperator div(t, new Ast::Literal(a1), new Ast::Literal(a2));
			Assert::IsTrue(*get_if<bool>(&div.run()) == expected);
		};

		check(Ast::BinaryOperator::Equal, 1, 1, true);
		check(Ast::BinaryOperator::Equal, 1, 2, false);

		check(Ast::BinaryOperator::Equal, string("asd"), string("asd"), true);
		check(Ast::BinaryOperator::Equal, string("asd"), string("aTd"), false);

		check(Ast::BinaryOperator::NotEqual, 1, 1, false);
		check(Ast::BinaryOperator::NotEqual, 1, 2, true);

		check(Ast::BinaryOperator::Less, 1, 2, true);
		check(Ast::BinaryOperator::Less, 3, 2, false);
		check(Ast::BinaryOperator::Less, 2, 2, false);

		check(Ast::BinaryOperator::LessEqual, 1, 2, true);
		check(Ast::BinaryOperator::LessEqual, 3, 2, false);
		check(Ast::BinaryOperator::LessEqual, 2, 2, true);

		check(Ast::BinaryOperator::Greater, 1, 2, false);
		check(Ast::BinaryOperator::Greater, 3, 2, true);
		check(Ast::BinaryOperator::Greater, 2, 2, false);

		check(Ast::BinaryOperator::GreaterEqual, 1, 2, false);
		check(Ast::BinaryOperator::GreaterEqual, 3, 2, true);
		check(Ast::BinaryOperator::GreaterEqual, 2, 2, true);
	}

	TEST_METHOD(If_n){
		{
			Ast::If_n n(new Ast::Literal(true), new Ast::Literal(3), new Ast::Literal(5));
			Value v = n.run();
			auto p = std::get_if<int>(&v);
			Assert::IsFalse(p == nullptr);
			Assert::IsTrue((*p) == 3);
		}
		{
			Ast::If_n n(new Ast::Literal(false), new Ast::Literal(3), new Ast::Literal(5));
			Value v = n.run();
			auto p = std::get_if<int>(&v);
			Assert::IsFalse(p == nullptr);
			Assert::IsTrue((*p) == 5);
		}
		{
			Ast::If_n n(new Ast::Literal(true), new Ast::Literal(3));
			Value v = n.run();
			auto p = std::get_if<int>(&v);
			Assert::IsFalse(p == nullptr);
			Assert::IsTrue((*p) == 3);
		}
		{
			Ast::If_n n(new Ast::Literal(false), new Ast::Literal(3));
			Value v = n.run();
			auto p = std::get_if<bool>(&v);
			Assert::IsFalse(p == nullptr);
			Assert::IsTrue((*p) == false);
		}
	}

	//Can't be tested without variables so will be tested in Ast-Environment integration tests
	//TEST_METHOD(While_n){}

	TEST_METHOD(Block){
		Ast::Node * l1 = new Ast::Literal(1);
		Ast::Node * l2 = new Ast::Literal(2);
		Ast::Node * l3 = new Ast::Literal(3);
		Ast::Node * l4 = new Ast::Literal(4);

		Ast::Block b({l1, l2, l3, l4});
		auto p = std::get_if<int>(&b.run());
		Assert::IsFalse(p == nullptr);
		Assert::IsTrue((*p) == 4);
	}
};
