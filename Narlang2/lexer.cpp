#include "pch.h"
#include "lexer.h"

const std::wstring ToString(Token t){
	switch(t){
	case Token::LeftBrace:
		return L"LeftBrace";
	case Token::RightBrace:
		return L"RightBrace";
	case Token::LeftParen:
		return L"LeftParen";
	case Token::RightParen:
		return L"RightParen";
	case Token::LeftBracket:
		return L"LeftBracket";
	case Token::RightBracket:
		return L"RightBracket";
	case Token::Plus:
		return L"Plus";
	case Token::Minus:
		return L"Minus";
	case Token::Star:
		return L"Star";
	case Token::Backslash:
		return L"Backslash";
	case Token::Equal:
		return L"Equal";
	case Token::NotEqual:
		return L"NotEqual";
	case Token::Greater:
		return L"Greater";
	case Token::GreaterEqual:
		return L"GreaterEqua";
	case Token::Less:
		return L"Less";
	case Token::LessEqual:
		return L"LessEqua";
	case Token::If:
		return L"If";
	case Token::Var:
		return L"Var";
	case Token::While:
		return L"While";
	case Token::Func:
		return L"Func";
	case Token::End:
		return L"End";
	case Token::Identifier:
		return L"Identifier";
	case Token::LiteralInt:
		return L"LiteralInt";
	case Token::LiteralBool:
		return L"LiteralBool";
	case Token::LiteralString:
		return L"LiteralString";
	case Token::Assign:
		return L"Assign";
	case Token::And:
		return L"And";
	case Token::Or:
		return L"Or";
	default:
		return L"TOKEN CONVERSION NOT IMPLEMENTED";
	}
}

TokenList::TokenList(){}

void TokenList::addToken(const Token token){
	tokens.push_back({token, false});
}

void TokenList::addToken(const Token token, const TokenData token_data){
	tokens.push_back({token, true});
	this->token_data.push_back(token_data);
}

void TokenList::setError(const std::string & error){
	this->error = error;
}

bool TokenList::hasError() const{
	return error.size() != 0;
}

const std::string TokenList::getError() const{
	return error;
}

Token TokenList::getToken() const{
	return tokens[current_token].first;
}

bool TokenList::hasToken() const{
	return current_token < tokens.size();
}

bool TokenList::hasTokenData() const{
	return tokens[current_token].second;
}

Token TokenList::peekToken() const{
	return tokens[current_token + 1].first;
}

TokenData TokenList::getTokenData() const{
	return token_data[current_token_data];
}

void TokenList::advanceToken() {
	if(tokens[current_token].second) current_token_data++;
	current_token++;
}

Lexer::Lexer(){}

const TokenList Lexer::parse(const std::string &source) const{
	TokenList token_list;

	int line_counter = 0;

	for(int i = 0; i < source.length(); ++i){
		switch(source.at(i)){
		case ' ':
		case '\t':
			break;
		case '\n':
			++line_counter;
			break;
		case '[':
			token_list.addToken(Token::LeftBracket);
			break;
		case ']':
			token_list.addToken(Token::RightBracket);
			break;
		case '(':
			token_list.addToken(Token::LeftParen);
			break;
		case ')':
			token_list.addToken(Token::RightParen);
			break;
		case '{':
			token_list.addToken(Token::LeftBrace);
			break;
		case '}':
			token_list.addToken(Token::RightBrace);
			break;
		case '+':
			token_list.addToken(Token::Plus);
			break;
		case '-':
			token_list.addToken(Token::Minus);
			break;
		case '\\':
			token_list.addToken(Token::Backslash);
			break;
		case '*':
			token_list.addToken(Token::Star);
			break;
		case ':':
			if(i < source.length() - 1 && source.at(i + 1) == '='){
				i++;
				token_list.addToken(Token::Assign);
			} else token_list.setError("Lexer error in line " + std::to_string(line_counter) + "\n ':' symbol. Did you mean ':='?\n");
			break;

		case '!':
			if(i < source.length() - 1 && source.at(i + 1) == '='){
				i++;
				token_list.addToken(Token::NotEqual);
			} else token_list.setError("Lexer error in line " + std::to_string(line_counter) + "\n '!' symbol. Did you mean '!='?\n");
			break;

		case '=':
			if(i + 1 < source.length() && source.at(i + 1) == '='){
				i++;
				token_list.addToken(Token::Equal);
			} else token_list.addToken(Token::Assign);
			break;

		case '>':
			if(i < source.length() - 1 && source.at(i + 1) == '='){
				i++;
				token_list.addToken(Token::GreaterEqual);
			} else token_list.addToken(Token::Greater);
			break;

		case '<':
			if(i < source.length() - 1 && source.at(i + 1) == '='){
				i++;
				token_list.addToken(Token::LessEqual);
			} else token_list.addToken(Token::Less);
			break;

		case '&':
			token_list.addToken(Token::And);
			break;

		case '|':
			token_list.addToken(Token::Or);
			break;

		default:
			auto isLetter = [](const char &c){
				return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
			};

			auto isDigit = [](const char &c){
				return (c >= '0' && c <= '9');
			};


			if(isLetter(source.at(i))){
				std::string word = "";
				for(; i < source.length() && isLetter(source.at(i)); ++i){
					word += source.at(i);
				}
				i--; //"This for loop"s to first NON letter character and then "main for loop" also advances i skipping character

				if(word == "if"){
					token_list.addToken(Token::If);
				} else if(word == "while"){
					token_list.addToken(Token::While);
				} else if(word == "var"){
					token_list.addToken(Token::Var);
				} else if(word == "print"){
					token_list.addToken(Token::Print);
				} else if(word == "func"){
					token_list.addToken(Token::Func);
				} else if(word == "true"){
					token_list.addToken(Token::LiteralBool, true);
				} else if(word == "false"){
					token_list.addToken(Token::LiteralBool, false);
				} else{
					token_list.addToken(Token::Identifier, word);
				}
			} else if(isDigit(source.at(i))){
				int number = 0;
				for(; i < source.length() && isDigit(source.at(i)); ++i){
					number *= 10;
					number += source.at(i) - '0';
				}
				i--; //"This for loop"s to first NON digit character and then "main for loop" also advances i skipping character
				token_list.addToken(Token::LiteralInt, number);
			} else if(source.at(i) == '\"'){
				i++;
				std::string word = "";
				for(; source.at(i) != '\"'; ++i){//this is correct because we should skip last '"' character
					word += source.at(i);
					if(source.at(i) == EOF){
						token_list.setError("Lexer error: Unclosed string\n");
						break;
					}
				}
				token_list.addToken(Token::LiteralString, word);
			} else{
				token_list.setError("Lexer error in line " + std::to_string(line_counter) + "\n Unknown symbol '" + source.at(i) + "'\n");
			}
			break;
		}
	}

	token_list.addToken(Token::End);

	return token_list;
}
