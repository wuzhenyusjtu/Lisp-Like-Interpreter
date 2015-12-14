#pragma once

#include <string>
#include <map>
#include <stack>

class SExp;

// Work as a call stack
class CallStack
{
private:
	std::map<std::string, SExp*> currentActivation;
	std::stack<std::map<std::string, SExp*> > funcActivationStack;
public:
	CallStack();
	~CallStack();
	void test();
	// Work as pushing parameters into call stack
	void addPairs(SExp* formalParams, SExp* actualParams);
	// Work as poping parameters into call stack
	void delPairs();
	// Check if the variable is bounded as a actual parameter
	bool boundParam(std::string paramName);
	// Get the value of the actual parameter
	SExp* getParamValue(std::string paramName);
};

