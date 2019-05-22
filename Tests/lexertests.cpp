#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Narlang2/lexer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(LexerTests){
public:
	void check_tokens(const std::string& s, const bool error, const std::vector<Token>& expected_tokens, 
							const std::vector<TokenData>& expected_token_data){
		auto tokens = lexer_parse(s);

		if (!error) {
			Assert::IsFalse(tokens.hasError(), stringToWchar(tokens.getError()));

			size_t data = 0;

			for(auto c : expected_tokens){
				Assert::AreEqual(c, tokens.getToken());
				if(tokens.hasTokenData()){
					Assert::IsTrue(expected_token_data[data] == tokens.getTokenData());
					data++;
				}
				tokens.advanceToken();
			}

			Assert::IsFalse(tokens.hasToken());
		}else {
			Assert::IsTrue(tokens.hasError(), stringToWchar(tokens.getError()));
		}
	};

	TEST_METHOD(Empty){
		check_tokens("", false, {Token::End}, {});
	}
		
	TEST_METHOD(Brackets) {
		check_tokens("{}[]()", false, { Token::LeftBrace, Token::RightBrace, Token::LeftBracket, Token::RightBracket, 
			Token::LeftParen, Token::RightParen, Token::End}, {});
	}

	TEST_METHOD(Keywords) {
		check_tokens("var while\nif \n func", false, {Token::Var, Token::While, Token::If, Token::Func, Token::End}, {});
	}

	TEST_METHOD(SpecialCharacters) {
		check_tokens("*\\+-==:=<><=>==&|", false, { Token::Star, Token::Backslash, Token::Plus, Token::Minus, Token::Equal, Token::Assign, 
			Token::Less, Token::Greater, Token::LessEqual, Token::GreaterEqual, Token::Assign, Token::And, Token::Or, Token::End}, {});
	}

	TEST_METHOD(UnkownSymbol){
		check_tokens("@", true, {}, {});
	}

	TEST_METHOD(LoneExcamationError){ 
		check_tokens("!", true, {}, {});
	}

	TEST_METHOD(LoneColonError) {
		check_tokens(":", true, {}, {});
    }

	TEST_METHOD(Identifiers) {
		check_tokens("asd gfhj \n yui", false, {Token::Identifier, Token::Identifier, Token::Identifier, Token::End},
			{std::string("asd"), std::string("gfhj"), std::string("yui")});
	}

	TEST_METHOD(IntLiterals){
		check_tokens("123 50005 \n 230023", false, {Token::LiteralInt, Token::LiteralInt, Token::LiteralInt, Token::End},
			{123, 50005, 230023});
	}

	TEST_METHOD(BoolLiterals){
		check_tokens("true false \n true", false, {Token::LiteralBool, Token::LiteralBool, Token::LiteralBool, Token::End},
			{true, false, true});
	}

	TEST_METHOD(StringLiterals){
		check_tokens("\"asd\"\"gfhj\" \n \"yui\"", false, {Token::LiteralString, Token::LiteralString, Token::LiteralString, Token::End},
			{std::string("asd"), std::string("gfhj"), std::string("yui")});
	}

	TEST_METHOD(GeneralTest1){
		check_tokens("\
			{\
				var i = 3\
				if(i*2 == 6){\
					print 4\
				}\
				\
			}\
			", false, {Token::LeftBrace, Token::Var, Token::Identifier, Token::Assign, Token::LiteralInt, 
			Token::If, Token::LeftParen, Token::Identifier, Token::Star, Token::LiteralInt, Token::Equal, Token::LiteralInt, 
			Token::RightParen, Token::LeftBrace, Token::Print, Token::LiteralInt, Token::RightBrace, Token::RightBrace, Token::End},
			{std::string("i"), 3, std::string("i"), 2, 6, 4});
	}

	TEST_METHOD(GeneralTest2){
		check_tokens("\
			{\
				var x = 0\
				while(x <= 10){\
					var y = 0\
					while(y < 20){\
						print x\
						print y\
						y = y + 1\
					}\
					x=x+1\
				}\
			}\
			", false, {	Token::LeftBrace,
						Token::Var, Token::Identifier, Token::Assign, Token::LiteralInt,
						Token::While, Token::LeftParen, Token::Identifier, Token::LessEqual, Token::LiteralInt, Token::RightParen, Token::LeftBrace,
						Token::Var, Token::Identifier, Token::Assign, Token::LiteralInt,
						Token::While, Token::LeftParen, Token::Identifier, Token::Less, Token::LiteralInt, Token::RightParen, Token::LeftBrace,
						Token::Print, Token::Identifier, Token::Print, Token::Identifier,
						Token::Identifier, Token::Assign, Token::Identifier, Token::Plus, Token::LiteralInt,
						Token::RightBrace,
						Token::Identifier, Token::Assign, Token::Identifier, Token::Plus, Token::LiteralInt,
						Token::RightBrace,
						Token::RightBrace,
						Token::End},			
			{std::string("x"), 0, std::string("x"), 10, std::string("y"), 0, std::string("y"), 20, std::string("x"), std::string("y"), 
			std::string("y"), std::string("y"), 1, std::string("x"), std::string("x"), 1});
	}
};
