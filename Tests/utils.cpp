#include "stdafx.h"

const wchar_t* stringToWchar(const std::string& s){
	std::wstringstream wstream;
	wstream << ("A" + s).c_str(); // Because first character is changed to weird one
	std::wstring ws = wstream.str();
	return ws.c_str();
}

#include "../Narlang2/ast.cpp"
#include "../Narlang2/lexer.cpp"
#include "../Narlang2/environment.cpp"