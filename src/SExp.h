#pragma once
#ifndef SEXP_H
#define SEXP_H

#include <string>
#include <stdexcept>
#include <cctype>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cstdlib>

using namespace std;

enum Type {BOOLEAN, NAT, LIST_NAT};

class Scanner;
class CallStack;
class CodeSegment;
class SExp {
private:
	string atom;
	SExp *lsexp;
	SExp *rsexp;
    SExp *psexp;
    bool isTail;
    bool isList;
	static SExp* car(SExp* s);
	static SExp* cdr(SExp* s);
	static SExp* cons(SExp* s1, SExp* s2);
	static SExp* isAtom(SExp* s);
	static SExp* isNull(SExp* s);
	static SExp* isInt(SExp* s);
	static SExp* isEqual(SExp* s1, SExp* s2);
	static SExp* isLess(SExp* s1, SExp* s2);
	static SExp* isGreater(SExp* s1, SExp* s2);
	static SExp* plus(SExp* s1, SExp* s2);
	static SExp* minus(SExp* s1, SExp* s2);
	static SExp* times(SExp* s1, SExp* s2);
	static SExp* quotient(SExp* s1, SExp* s2);
	static SExp* remainder(SExp* s1, SExp* s2);
	static void checkActualParameters(int paramsNum, SExp* s, std::string funcName);
	static bool isLeaf(SExp* s);
	static bool isNumeric(string literal);
    static string intToString(int value);
    static int stringToInt(string s);
public:
	SExp();
	SExp(string literal, SExp *parent);
    SExp(string literal);
    SExp(SExp *s1, SExp *s2);
	SExp(SExp *s1, SExp *s2, SExp *parent);
	~SExp();
	string getAtom();
	SExp* getlsexp();
	SExp* getrsexp();
	void parse();
    static Type typeCheck(SExp* s);
    static Type typeCheckCond(SExp* s);
	static SExp* evaluate(SExp* s);
	static SExp* evalCond(SExp* s);
	static SExp* evalList(SExp* s);
	static SExp* apply(SExp* s, SExp* list);
	void computeAttribute(bool *isSexpList);
	void printDotNotation();
	void printListNotation(bool isLeftChild);
	friend class CallStack;
	friend class CodeSegment;
};
#endif