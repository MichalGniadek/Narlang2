#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Narlang2/environment.h"
#include "../Narlang2/environment.cpp"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std;
using namespace Env;

TEST_CLASS(EnvironmentTests){
public:
	TEST_METHOD(SetGetOneValue){
		Environment e;
		e.increaseScope();
		string id = "a";
		Ast::Value v = 3;
		e.allocateValue(id, v);
		Assert::IsTrue(*std::get_if<int>(&v) == *std::get_if<int>(&e.getValue(id)));
	}

	TEST_METHOD(MultipleValues){
		Environment e;
		e.increaseScope();
		string id1 = "a";
		Ast::Value v1 = 3;
		string id2 = "b";
		Ast::Value v2 = 5;
		e.allocateValue(id1, v1);
		e.allocateValue(id2, v2);
		Assert::IsTrue(*std::get_if<int>(&v1) == *std::get_if<int>(&e.getValue(id1)));
		Assert::IsTrue(*std::get_if<int>(&v2) == *std::get_if<int>(&e.getValue(id2)));
	}

	TEST_METHOD(Scopes){
		Environment e;
		e.increaseScope();
		string id = "a";
		Ast::Value v1 = 3;
		Ast::Value v2 = 5;
		e.allocateValue(id, v1);
		e.increaseScope();
		e.allocateValue(id, v2);
		Assert::IsTrue(*std::get_if<int>(&v2) == *std::get_if<int>(&e.getValue(id)));
		e.decreaseScope();
		Assert::IsTrue(*std::get_if<int>(&v1) == *std::get_if<int>(&e.getValue(id)));
	}
};
