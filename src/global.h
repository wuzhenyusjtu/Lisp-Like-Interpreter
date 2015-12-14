#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

#include <fstream>
#include <map>
#include <string>
#include "Primitive.h"
#include "SExp.h"

class Scanner;
class CallStack;
class CodeSegment;

extern Scanner *scanner;
// extern std::ofstream out;
extern std::map<std::string, Primitive> primitiveMap;
// Simulate the call stack in calling functions
extern CallStack *callStack;
// Simulate the code segment in storing function definition
extern CodeSegment *codeSegment;
#endif