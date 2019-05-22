#pragma once
#include <vector>
#include <variant>
#include <string>

enum class Token{	LeftBrace, RightBrace,
					LeftParen, RightParen,
					LeftBracket, RightBracket,
					Plus, Minus, Star, Backslash,
					Equal, NotEqual, Greater, GreaterEqual, Less, LessEqual,
					Assign,
					And, Or,
					If, Var, While, Func, Print, End,
					Identifier, LiteralInt, LiteralBool, LiteralString};

const std::wstring ToString(Token t);

using TokenData = std::variant<std::string, int, bool>;

class TokenList{
public:
	TokenList();

	void addToken(const Token token);
	void addToken(const Token token, const TokenData token_data);
	void setError(const std::string& error);

	bool hasError() const;
	const std::string getError() const;
	Token getToken() const;
	bool hasToken() const;
	bool hasTokenData() const;
	void advanceToken();
	Token peekToken() const;
	TokenData getTokenData() const;
private:
	using advanceData = bool;
	std::vector<std::pair<Token, advanceData>> tokens;
	size_t current_token = 0;

	std::vector<TokenData> token_data;
	size_t current_token_data = 0;

	std::string error;
};

TokenList lexer_parse(const std::string& source);