#pragma once
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

enum TokenType { OpeningParenthesis, ClosingParenthesis, Dot, Atom, Eof };

class Token
{
private:
	string literal;
	TokenType type;
public:
	Token(string l, TokenType t);
	string getLiteral();
	TokenType getType();
	~Token();
};
#endif