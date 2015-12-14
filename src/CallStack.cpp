#include "CallStack.h"
#include "SExp.h"


CallStack::CallStack()
{
}


CallStack::~CallStack()
{
}

void CallStack::test() 
{
	typedef std::map<std::string, SExp*>::iterator it_type;
	for (it_type iterator = currentActivation.begin(); iterator != currentActivation.end(); iterator++) {
		iterator->second->printDotNotation();
		cout << endl;
	}
}

void CallStack::addPairs(SExp* formalList, SExp* actualList) {
	map<string, SExp*> association;
	SExp* formal;
	SExp* actual;
	// Check if the number of actual parameters matches the number of formal parameters
	while (SExp::isNull(formalList)->getAtom() == "NIL" && SExp::isNull(actualList)->getAtom() == "NIL")
	{
		formal = SExp::car(formalList);
		// Check if formal parameter is Atom
		if(SExp::isAtom(formal)->getAtom() == "NIL")
		{
			cout << "ERROR: Formal Parameters should be Atoms!" << endl;
			exit(0);
		}
		actual = SExp::car(actualList);
		formalList = SExp::cdr(formalList);
		actualList = SExp::cdr(actualList);
		association.insert(make_pair(formal->getAtom(), actual));
		//std::cout << formal->getAtom() << std::endl;
		//std::cout << actual->getAtom() << std::endl;
	}
	if (SExp::isNull(formalList)->getAtom() != SExp::isNull(actualList)->getAtom())
	{
		cout << "ERROR: Number of Actual Parameters doesn't match Number of Formal Parameters!" << endl;
		exit(0);
	}
	funcActivationStack.push(association);
	currentActivation = funcActivationStack.top();
}

void CallStack::delPairs() {
	funcActivationStack.pop();
	if (!funcActivationStack.empty())
	{
		currentActivation = funcActivationStack.top();
	}
}

bool CallStack::boundParam(std::string paramName) {
	if (currentActivation.find(paramName) != currentActivation.end())
		return true;
	return false;
}

SExp* CallStack::getParamValue(std::string paramName) {
	return currentActivation.find(paramName)->second;
}