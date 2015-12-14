#pragma once
#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <cstdlib>

using namespace std;

class Token;

class Scanner {
private:
	int count;
	vector<Token> tokenVec;
	bool isClosingParenthesis(char c);
	bool isOpeningParenthesis(char c);
	bool isDot(char c);
	bool isDigit(char c);
	bool isUpperCaseLetter(char c);
	bool isAtomValid(string s);
	void getTokensFromString(string s);
public:
	Scanner(vector<string> svec);
	vector<Token> getTokenVec();
	void nextToken();
	void prevToken();
	Token getCurrentToken();
	void test();
	~Scanner();
};
#endif