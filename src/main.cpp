#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>

#include "Token.h"
#include "Start.h"
#include "SExp.h"
#include "Scanner.h"
#include "global.h"
#include "Primitive.h"
#include "CallStack.h"
#include "CodeSegment.h"


void initPrimitiveMap();

int main(int argc, char * argv[]) {
	std::string infile = argv[1];
    std::cout << infile << std::endl;
	//string outfile = argv[2];
	ifstream in;
	in.open(infile.c_str(), ifstream::in);
	//out.open(outfile.c_str(), ofstream::out);
	std::string line;
	vector<string> svec;
	while (getline(in, line))
	{
		line += '\n';
		svec.push_back(line);
	}
	in.close();
	initPrimitiveMap();
	scanner = new Scanner(svec);
	//scanner->test();

	callStack = new CallStack();
	codeSegment = new CodeSegment();
	Start start = Start();
	start.parse();
    start.typeCheck();
	start.computeAttribute();
	cout << "Printing Result:" << endl;
	start.print();

	std::cout << "Evaluating Result:" << std::endl;
	start.evaluate();
	//codeSegment->test();
	//callStack->test();
	
}

void initPrimitiveMap() {
	primitiveMap.insert(std::make_pair("CAR", CAR));
	primitiveMap.insert(std::make_pair("CDR", CDR));
	primitiveMap.insert(std::make_pair("CONS", CONS));
	primitiveMap.insert(std::make_pair("ATOM", ATOM));
	primitiveMap.insert(std::make_pair("EQ", EQ));
	primitiveMap.insert(std::make_pair("NULL", NUL));
	primitiveMap.insert(std::make_pair("INT", INT));
	primitiveMap.insert(std::make_pair("PLUS", PLUS));
	primitiveMap.insert(std::make_pair("MINUS", MINUS));
	primitiveMap.insert(std::make_pair("TIMES", TIMES));
	primitiveMap.insert(std::make_pair("QUOTIENT", QUOTIENT));
	primitiveMap.insert(std::make_pair("REMAINDER", REMAINDER));
	primitiveMap.insert(std::make_pair("LESS", LESS));
	primitiveMap.insert(std::make_pair("GREATER", GREATER));
	primitiveMap.insert(std::make_pair("COND", COND));
	primitiveMap.insert(std::make_pair("QUOTE", QUOTE));
	primitiveMap.insert(std::make_pair("DEFUN", DEFUN));
}
