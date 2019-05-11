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
		auto p = std::get_if<Ast::Null>(&e.getValue("a"));
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
};
