#include "Start.h"
#include "SExp.h"
#include  "Token.h"
#include "Scanner.h"
#include "global.h"

Start::Start(){ sexp = NULL; start = NULL; isSexpList = true; }

Start::~Start() {}

void Start::parse() {
	sexp = new SExp();
	sexp->parse();
	scanner->nextToken();
	if ((scanner->getCurrentToken()).getType() == Eof)
		return;
	else {
		scanner->prevToken();
		start = new Start();
		start->parse();
	}
}

void Start::typeCheck(){
    SExp::typeCheck(sexp);
    if(start != NULL)
    {
        start->typeCheck();
    }
}

void Start::evaluate() {
    SExp* s=SExp::evaluate(sexp);
	//s->printDotNotation();
	bool isSList = true;
	s->computeAttribute(&isSList);
	if (!isSList)
	{
		s->printDotNotation();
	}
	else {
		s->printListNotation(true);
	}
	cout << endl;
	if (start != NULL)
	{
		start->evaluate();
	}
}

void Start::computeAttribute() {
	sexp->computeAttribute(&isSexpList);
	if (start != NULL)
		start->computeAttribute();
}

void Start::print() {
	if (!isSexpList)
	{
		printDotNotation();
	}
	else {
		printListNotation();
	}
}

void Start::printDotNotation() {
	sexp->printDotNotation();
	cout << endl;
	if (start != NULL)
	{
		start->print();
	}
}

void Start::printListNotation() {
	sexp->printListNotation(true);
	cout << endl;
	if (start != NULL)
	{
		start->print();
	}
}

