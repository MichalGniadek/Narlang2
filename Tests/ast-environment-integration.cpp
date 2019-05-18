#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Narlang2/ast.h"
#include "../Narlang2/environment.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std;

TEST_CLASS(Ast_Environment_Integration){
public:
	TEST_METHOD(Var){
		Env::Environment e;
		e.increaseScope();
		Ast::Var v("a");
		v.run(e);
		auto p = std::get_if<Ast::Null>(&e.getValue(Ast::Identifier("a")));
		Assert::IsFalse(p == nullptr);
	}

	TEST_METHOD(Identifier){
		Env::Environment e;
		e.increaseScope();
		Ast::Var v("a");
		v.run(e);
		Ast::Identifier_n id("a");
		auto p = std::get_if<Ast::Identifier>(&id.run(e));
		Assert::IsFalse(p == nullptr);
	}

	TEST_METHOD(Assign){
		Env::Environment e;
		e.increaseScope();
		Ast::Var v("a");
		v.run(e);
		
		Ast::Node* id = new Ast::Identifier_n("a");
		Ast::Node* l = new Ast::Literal(3);
		Ast::Assign as(id, l);

		auto p = std::get_if<int>(&as.run(e));
		Assert::IsFalse(p == nullptr);
		Assert::IsTrue(*(int*)p == 3);
	}

	TEST_METHOD(While_n){
		Env::Environment e;
		e.increaseScope();

		Ast::Block b({
			new Ast::Assign(
				new Ast::Var("i"),
				new Ast::Literal(0)
			),

			new Ast::Assign(
				new Ast::Identifier_n("i"),
				new Ast::BinaryOperator(Ast::BinaryOperator::Addition, new Ast::Identifier_n("i"), new Ast::Literal(1))
			),

			new Ast::While_n(
				new Ast::BinaryOperator(Ast::BinaryOperator::Less, new Ast::Identifier_n("i"), new Ast::Literal(5)),
				new Ast::Assign(
					new Ast::Identifier_n("i"),
					new Ast::BinaryOperator(Ast::BinaryOperator::Addition, new Ast::Identifier_n("i"), new Ast::Literal(1))
				)
			),

			new Ast::DEBUG_GetValue(new Ast::Identifier_n("i"))
		});
		
		Assert::IsTrue(*std::get_if<int>(&b.run(e)) == 5);
	}
};
