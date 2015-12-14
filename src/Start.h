#pragma once
#ifndef START_H
#define START_H

#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

class SExp;

class Start {
private:
	SExp *sexp;
	Start *start;
	bool isSexpList;
public:
	Start();
	~Start();
	void parse();
	void evaluate();
    void typeCheck();
	void computeAttribute();
	void print();
	void printDotNotation();
	void printListNotation();
};
#endif