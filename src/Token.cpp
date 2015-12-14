#include "Token.h"

Token::Token(string l, TokenType t)
{
	this->literal = l;
	this->type = t;
}

string Token::getLiteral() { return this->literal; }

TokenType Token::getType() { return this->type; }

Token::~Token(){}
