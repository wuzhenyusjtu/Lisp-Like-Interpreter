#pragma once
#include <string>
#include <map>
#include <unordered_set>

class SExp;

class CodeSegment
{
private:
	// Work as a code segment
	std::map<std::string, SExp*> definitionList;
	void checkFormalParamsName(SExp*);
public:
	CodeSegment();
	~CodeSegment();
	void test();
	// Check if the functions is defined
	bool boundFunc(std::string funcName);
	// Get the definition of the funtion
	SExp* getFuncDef(std::string funcName);
	// Insert the definition
	void insertFuncDef(std::string funName, SExp* funcdef);
};

