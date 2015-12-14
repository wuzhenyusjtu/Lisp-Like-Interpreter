#include "Scanner.h"
#include "Token.h"
#include <iostream>
#include "global.h"

bool Scanner::isClosingParenthesis(char c) { return c == ')'; }

bool Scanner::isOpeningParenthesis(char c) { return c == '('; }

bool Scanner::isDot(char c) { return c == '.'; }

bool Scanner::isDigit(char c) { return c >= '0' && c <= '9'; }

bool Scanner::isUpperCaseLetter(char c) { return c >= 'A' && c <= 'Z'; }

bool Scanner::isAtomValid(string s) {
	if (isDigit(s[0]))
	{
		for (int i = 1; (int)i < s.size(); i++)
		{
			if (!isDigit(s[i])) return false;
		}
	}
	return true;
}

void Scanner::getTokensFromString(string s) {
	string literal = "";
	for (int i = 0; i < (int)s.size(); i++)
	{
		if (isOpeningParenthesis(s[i])) {
			if (literal != "") {
				if (isAtomValid(literal))
				{
					tokenVec.push_back(Token(literal, Atom));
					literal = "";
				}
				else {
					cout << "ERROR: Invalid Atom Format, Atom should be either a literal atom of digits and upper-case letters, or a numeric" << endl;
					exit(0);
				}
			}
			Token token = Token("(", OpeningParenthesis);
			tokenVec.push_back(token);
		}
		else if (isClosingParenthesis(s[i])) {
			if (literal != "") {
				if (isAtomValid(literal))
				{
					tokenVec.push_back(Token(literal, Atom));
					literal = "";
				}
				else {
					cout << "ERROR: Invalid Atom Format, Atom should be either a literal atom of digits and upper-case letters, or a numeric"<<endl;
					exit(0);
				}
			}
			Token token = Token(")", ClosingParenthesis);
			tokenVec.push_back(token);
		}
		else if (isDot(s[i])) {
			if (literal != "") {
				if (isAtomValid(literal))
				{
					tokenVec.push_back(Token(literal, Atom));
					literal = "";
				}
				else {
					cout << "ERROR: Invalid Atom Format, Atom should be either a literal atom of digits and upper-case letters, or a numeric" << endl;
					exit(0);
				}
			}
			Token token = Token(".", Dot);
			tokenVec.push_back(token);
		}
		else if (isDigit(s[i]) || isUpperCaseLetter(s[i])) {
			literal += s[i];
		}
		else if (std::isspace(s[i])) {
			if (literal != "") {
				if (isAtomValid(literal))
				{
					tokenVec.push_back(Token(literal, Atom));
					literal = "";
				}
				else {
					cout << "ERROR: Invalid Atom Format, Atom should be either a literal atom of digits and upper-case letters, or a numeric" << endl;
					exit(0);
				}
			}
		}
		else {
			cout << "ERROR: Invalid Input Stream, the input characters should be digits, upper-case letters, opening, closing parenthesis, dot and white spaces" << endl;
			exit(0);
		}
	}
}

Scanner::Scanner(vector<string> svec) {
	count = -1;
	for (vector<string>::iterator it = svec.begin(); it != svec.end(); it++)
	{
		getTokensFromString(*it);
	}
	tokenVec.push_back(Token("EOF", Eof));
}

vector<Token> Scanner::getTokenVec() { return tokenVec; }

void Scanner::nextToken() {
	count++;
}

void Scanner::prevToken() {
	count--;
}

void Scanner::test() {
	for (int i = 0; i < tokenVec.size(); i++)
	{
		cout << tokenVec[i].getLiteral();
	}
}

Token Scanner::getCurrentToken() {
	if (count < (int)tokenVec.size())
	{
		return tokenVec[count];
	}
	else {
		cout << "ERROR: Token Stream Overflow" << endl;
		exit(0);
	}
}

Scanner::~Scanner()
{
}