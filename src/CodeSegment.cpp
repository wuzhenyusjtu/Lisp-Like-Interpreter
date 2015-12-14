#include "CodeSegment.h"
#include "SExp.h"


CodeSegment::CodeSegment()
{
}


CodeSegment::~CodeSegment()
{
}

void CodeSegment::checkFormalParamsName(SExp* formalList){
	unordered_set<std::string> formalsSet;
	while(SExp::isNull(formalList)->getAtom() == "NIL"){
        if(SExp::car(formalList)->getAtom() == "NIL" || SExp::car(formalList)->getAtom() == "T")
		{
			std::cout << "ERROR: Formal Parameter Name Cannot collide with \"T\" or \"NIL\"" << std::endl;
			exit(0);
		}
		if (formalsSet.find(SExp::car(formalList)->getAtom())==formalsSet.end())
			formalsSet.insert(SExp::car(formalList)->getAtom());
		else {
			std::cout << "ERROR: Formal Parameter Name Collision! Cannot have formal paramters with the same name!" << endl; 
			exit(0);
		}
		formalList = SExp::cdr(formalList);
	}	
}
void CodeSegment::test() 
{
	typedef std::map<std::string, SExp*>::iterator it_type;
	for (it_type iterator = definitionList.begin(); iterator != definitionList.end(); iterator++) {
		iterator->second->printDotNotation();
		cout << endl;
	}
}

bool CodeSegment::boundFunc(std::string funcName) 
{
	if (definitionList.find(funcName) == definitionList.end())
		return false;
	return true;
}

SExp* CodeSegment::getFuncDef(std::string funcName) 
{
	return definitionList.find(funcName)->second;
}

void CodeSegment::insertFuncDef(std::string funcName, SExp* funcdef)
{
	// Check the formal parameter list, if there exits name collision
	checkFormalParamsName(SExp::car(funcdef));
	definitionList.insert(std::make_pair(funcName, funcdef));
}