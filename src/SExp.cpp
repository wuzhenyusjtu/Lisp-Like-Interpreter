#include "SExp.h"
#include "Token.h"
#include "Scanner.h"
#include "global.h"
#include "CallStack.h"
#include "CodeSegment.h"

SExp::SExp() { lsexp = NULL; rsexp = NULL; psexp = NULL; isList = false; isTail = false; }

SExp::SExp(string literal, SExp *parent) { atom = literal; lsexp = NULL; rsexp = NULL; psexp = parent; isList = false; isTail = false; }

SExp::SExp(string literal) { atom = literal; lsexp = NULL; rsexp = NULL;isList = false; isTail = false; }

SExp::SExp(SExp *s1, SExp *s2, SExp *parent) { lsexp = s1; rsexp = s2; psexp = parent; isList = false; isTail = false; }

SExp::SExp(SExp *s1, SExp *s2) { lsexp = s1; rsexp = s2; isList = false; isTail = false; }

SExp::~SExp() {}

void SExp::parse() {
	scanner->nextToken();
	if ((scanner->getCurrentToken()).getType() == OpeningParenthesis)
	{
		lsexp = new SExp();
		lsexp->parse();
		scanner->nextToken();
		if ((scanner->getCurrentToken()).getType() == Dot)
		{
			rsexp = new SExp();
			rsexp->parse();
			scanner->nextToken();
			if ((scanner->getCurrentToken()).getType() == ClosingParenthesis)
				return;
			else {
				cout << "ERROR: Expected ClosingParenthesis"<<endl;
				exit(0);
			}
		}
		else {
			cout << "ERROR: Expected Dot" << endl;
			exit(0);
		}
	}
	else if ((scanner->getCurrentToken()).getType() == Atom) {
		atom = (scanner->getCurrentToken()).getLiteral();
		return;
	}
	else {
		cout << "ERROR: Expected OpeningParenthesis or Atom" << endl;
		exit(0);
	}
}

string SExp::getAtom() {
	return atom;
}

SExp* SExp::getlsexp() {
	return lsexp;
}

SExp* SExp::getrsexp() {
	return rsexp;
}

Type SExp::typeCheckCond(SExp *s){
    if (isNull(s)->getAtom() == "T") {
        std::cout << "ERROR: Cond should not have empty content!" << std::endl;
        exit(0);
    }
    if (typeCheck(car(car(s)))!=BOOLEAN) {
        std::cout << "ERROR: Cond should have BOOLEAN expressions!" << std::endl;
        exit(0);
    }
    Type type = typeCheck(car(cdr(car(s))));
    if (isNull(cdr(s))->getAtom()=="T") {
        return type;
    }
    else{
        if (type != typeCheckCond(cdr(s))){
            std::cout << "ERROR: Cond expression parts can have any of the three types NAT, BOOLEAN, and LIST_NAT! But should be of the same type!" << std::endl;
            exit(0);
        }
        else return type;
    }
}

Type SExp::typeCheck(SExp *s){
    if (isAtom(s)->getAtom() == "T") {
        if (s->getAtom()== "T")
            return BOOLEAN;
        else if(s->getAtom()=="F")
            return BOOLEAN;
        else if(isInt(s)->getAtom()=="T")
            return NAT;
        else if(s->getAtom()=="NIL")
            return LIST_NAT;
        else{
            std::cout << "ERROR: Atom type unmatched! We only support BOOLEAN, NAT and LIST_NAT!"<< std::endl; exit(0);
        }
    }
    if (primitiveMap.find(car(s)->getAtom())!=primitiveMap.end()) {
            Primitive primitive = primitiveMap.find(car(s)->getAtom())->second;
            switch (primitive) {
                case COND:
                    return typeCheckCond(cdr(s));
                    break;
                case CAR:
                    if (typeCheck(car(cdr(s)))==LIST_NAT) {
                        return NAT;
                    }
                    std::cout << "ERROR: CAR expected one parameter with type of LIST(NAT)!" << std::endl;
                    exit(0);
                    break;
                case CDR:
                    if (typeCheck(car(cdr(s)))==LIST_NAT) {
                        return LIST_NAT;
                    }
                    std::cout << "ERROR: CDR expected one parameter with type of LIST(NAT)!" << std::endl;
                    exit(0);
                    break;
                case CONS:
                    if (typeCheck(car(cdr(s)))==NAT && typeCheck(car(cdr(cdr(s))))==LIST_NAT) {
                        return LIST_NAT;
                    }
                    std::cout << "ERROR: CONS expected two parameters, 1st NAT and 2nd LIST(NAT)!" << std::endl;
                    exit(0);
                    break;
                case ATOM:
					typeCheck(car(cdr(s)));
                    return BOOLEAN;
                    break;
                case EQ:
                    if (typeCheck(car(cdr(s)))==NAT && typeCheck(car(cdr(cdr(s))))==NAT) {
                        return BOOLEAN;
                    }
                    std::cout << "ERROR: EQ expected two parameters with type of NAT!" << std::endl;
                    exit(0);
                    break;
                case NUL:
                    if (typeCheck(car(cdr(s)))==LIST_NAT) {
                        return BOOLEAN;
                    }
                    std::cout << "ERROR: NULL expected parameter with type of LIST(NAT)!" << std::endl;
                    exit(0);
                case INT:
					typeCheck(car(cdr(s)));
                    return BOOLEAN;
                    break;
                case PLUS:
                    if (typeCheck(car(cdr(s)))==NAT && typeCheck(car(cdr(cdr(s))))==NAT) {
                        return NAT;
                    }
                    std::cout << "ERROR: PLUS expected two parameters with type of NAT!" << std::endl;
                    exit(0);
                    break;
                case LESS:
                    if (typeCheck(car(cdr(s)))==NAT && typeCheck(car(cdr(cdr(s))))==NAT) {
                        return BOOLEAN;
                    }
                    std::cout << "ERROR: LESS expected two parameters with type of NAT!" << std::endl;
                    exit(0);
                    break;
                default:
                    std::cout << "ERROR: Type Checking Only Supports Limited Subset of LISP Behavior!" << std::endl;
                    exit(0);
                    break;
            }
    }
    else {
        std::cout << "ERROR: Type Checking Only Supports Limited Subset of LISP Behavior!" << std::endl;
        exit(0);
    }
    
}

SExp* SExp::evaluate(SExp* s) {
	if (isAtom(s)->getAtom() == "T") {
		if (isInt(s)->getAtom() == "T") return s;
		if (s->getAtom() == "T") return new SExp("T");
		if (s->getAtom() == "NIL") return new SExp("NIL");
		//std::cout << s->getAtom() << std:: endl;
		if (callStack->boundParam(s->getAtom())) return callStack->getParamValue(s->getAtom());
		else {
			cout << "ERROR: Unbounded function Parameter! We don't have matching value for it!" << endl;
			exit(0);
		}
	}
	else {
		//std::cout << car(s)->getAtom() << std::endl;
		if (primitiveMap.find(car(s)->getAtom())!=primitiveMap.end())
		{
			Primitive primitive = primitiveMap.find(car(s)->getAtom())->second;
			switch (primitive)
			{
			case COND:
				return evalCond(cdr(s));
				break;
			case QUOTE:
				checkActualParameters(1, cdr(s), car(s)->getAtom());
				return car(cdr(s));
				break;
			case DEFUN:
				// Check if functions definition has exactly three parameters
				checkActualParameters(3, cdr(s), car(s)->getAtom());
				// Check if function name is atom
				if(isAtom(car(cdr(s)))->getAtom() == "NIL"){
					cout << "ERROR: Function Name should be an atom!" << endl;
					exit(0);
				}
				// Check if function name is non-integer
				if(isInt(car(cdr(s)))->getAtom() == "T")
				{
					cout << "ERROR: Function Name should not be an integer!" << endl;
					exit(0);
				}
				// Check if function name conflicts with inner-built functions
				if (primitiveMap.find(car(cdr(s))->getAtom())!=primitiveMap.end()){
					cout << "ERROR: Function Name Collision with Built-in Primitive!" << endl;
					exit(0);
				}
				// Check if function is already defined
				if (!codeSegment->boundFunc(car(cdr(s))->getAtom()))
				{
					codeSegment->insertFuncDef(car(cdr(s))->getAtom(), cdr(cdr(s)));
					return new SExp(car(cdr(s))->getAtom());
				}
				else {
					cout << "ERROR: Function " << car(cdr(s))->getAtom() << " Redefined!" << endl;
					exit(0);
				}
			default:
				break;
			}
		}
		return apply(car(s), evalList(cdr(s)));
	}
}

// To Assert parameters number matching requirement
SExp* SExp::apply(SExp* s, SExp* list) {
	if (isAtom(s)->getAtom()=="T")
	{
		if (primitiveMap.find(s->getAtom()) != primitiveMap.end())
		{
			Primitive primitive = primitiveMap.find(s->getAtom())->second;

			switch (primitive)
			{
			case CAR:
				checkActualParameters(1, list, s->getAtom());
				return car(car(list));
				break;
			case CDR:
				checkActualParameters(1, list, s->getAtom());
				return cdr(car(list));
				break;
			case CONS:
				checkActualParameters(2, list, s->getAtom());
				return cons(car(list), car(cdr(list)));
				break;
			case ATOM:
				checkActualParameters(1, list, s->getAtom());
				return isAtom(car(list));
				break;
			case EQ:
				checkActualParameters(2, list, s->getAtom());
				return isEqual(car(list), car(cdr(list)));
				break;
			case NUL:
				checkActualParameters(1, list, s->getAtom());
				return isNull(car(list));
				break;
			case INT:
				checkActualParameters(1, list, s->getAtom());
				return isInt(car(list));
				break;
			case PLUS:
				checkActualParameters(2, list, s->getAtom());
				return plus(car(list), car(cdr(list)));
				break;
			case MINUS:
				checkActualParameters(2, list, s->getAtom());
				return minus(car(list), car(cdr(list)));
				break;
			case TIMES:
				checkActualParameters(2, list, s->getAtom());
				return times(car(list), car(cdr(list)));
				break;
			case QUOTIENT:
				checkActualParameters(2, list, s->getAtom());
				return quotient(car(list), car(cdr(list)));
				break;
			case REMAINDER:
				checkActualParameters(2, list, s->getAtom());
				return remainder(car(list), car(cdr(list)));
				break;
			case LESS:
				checkActualParameters(2, list, s->getAtom());
				return isLess(car(list), car(cdr(list)));
				break;
			case GREATER:
				checkActualParameters(2, list, s->getAtom());
				return isGreater(car(list), car(cdr(list)));
				break;
			default:
				cout << "BUG IDENTIFIED : NO PRIMITIVE MATCHING FOR APPLYING!" << endl;
				exit(0);
			}
		}
		else {
			if (codeSegment->boundFunc(s->getAtom()))
			{
				callStack->addPairs(car(codeSegment->getFuncDef(s->getAtom())), list);
				SExp* ret = evaluate(car(cdr((codeSegment->getFuncDef(s->getAtom())))));
				callStack->delPairs();
				return ret;
			}
			else {
				cout << "ERROR: Function " << s->getAtom() << " Undefined!" << endl;
				exit(0);
			}
		}
	}
	else{
		cout << "ERROR: Here cannont have non-atomic expr in (expr ...)!" << endl;
		exit(0);
	};
}

void SExp::checkActualParameters(int paramsNum, SExp* list, std::string funcName){
	int parameterCount=0;
	while (isNull(list)->getAtom()!="T"){
		list=cdr(list);
		parameterCount++;
	}
	if (parameterCount!=paramsNum) {
		cout << "ERROR: "<< funcName <<" Function should have exactly " << paramsNum << " paremeters!" << endl;	
		exit(0);
	}
}

SExp* SExp::evalCond(SExp* s) {
	if (isNull(s)->getAtom() == "T"){ cout <<"ERROR: Cannont Have Empty List of Condition-Value Pairs!";exit(0);}
	else
	{
		if (evaluate(car(car(s)))->getAtom() == "T") return evaluate(car(cdr(car(s))));
		return evalCond(cdr(s));
	}
}

SExp* SExp::evalList(SExp* s) {
	if (isNull(s)->getAtom() == "T") return new SExp("NIL");
	else return cons(evaluate(car(s)),evalList(cdr(s)));
}

void SExp::computeAttribute(bool *isSexpList) {
	if (lsexp != NULL && rsexp != NULL)
	{
		lsexp->computeAttribute(isSexpList);
		rsexp->computeAttribute(isSexpList);
		if (rsexp->isList == true) {
			isList = true;
		}
		else {
			isList = false;
		}
		if (!isList)
		{
			*isSexpList = false;
		}
	}
	else if(lsexp == NULL && rsexp == NULL)
    {
		if (atom == "NIL") {
			isList = true;
		}
		else {
			isList = false;
		}
	}
    else {
        std::cout << "BUG IDENTIFIED: Atom Node NOT Initialized with NULL Pointers!" << std::endl;
        exit(0);
    }
}

void SExp::printDotNotation() {
	if (lsexp != NULL)
	{
		cout << "(";
		lsexp->printDotNotation();
		cout << " . ";
		rsexp->printDotNotation();
		cout << ")";
	}
	else
		cout << atom;
}

void SExp::printListNotation(bool isLeftChild) {
	if (isLeftChild)
	{
		if (isList && atom != "NIL")
		{
			cout << '(';
			lsexp->printListNotation(true);
			rsexp->printListNotation(false);
			cout << ')';
		}
		else {
			cout << atom << ' ';
		}
	}
	else
	{
		if (atom != "NIL")
		{
			lsexp->printListNotation(true);
			rsexp->printListNotation(false);
		}
	}
}

SExp* SExp::cdr(SExp* s) {
	if (s->getrsexp() == NULL) {
		cout << "ERROR: CDR Operation Failed! Cannont Work On Atom!" << endl;
		exit(0);
	};
	return s->getrsexp();
}

SExp* SExp::car(SExp* s) {
	if (s->getlsexp() == NULL) {
		cout << "ERROR: CAR Operation Failed! Cannont Work On Atom!" << endl;
		exit(0);
	}
	return s->getlsexp();
}

SExp* SExp::cons(SExp* s1, SExp* s2) {
	return new SExp(s1, s2);
}

SExp* SExp::isAtom(SExp* s) {
	if (isLeaf(s)){
		return new SExp("T");
	}
	else return new SExp("NIL");
}

SExp* SExp::isNull(SExp* s) {
	if (isLeaf(s) && s->getAtom() == "NIL")
	{
		return new SExp("T");
	}
	else return new SExp("NIL");
}

SExp* SExp::isInt(SExp* s) {
	if (isLeaf(s) && isNumeric(s->getAtom()))
	{
		return new SExp("T");
	}
	else return new SExp("NIL");
}

SExp* SExp::isEqual(SExp* s1, SExp* s2) {
	if (isLeaf(s1) && isLeaf(s2))
		if (s1->getAtom() == s2->getAtom()) return new SExp("T");
		else return new SExp("NIL");
	else {
		cout << "ERROR: EQ function only works on a pair of atoms!" << endl;
		exit(0);
	}
}

SExp* SExp::isLess(SExp* s1, SExp* s2) {
	if (isLeaf(s1) && isLeaf(s2)){
		if (isNumeric(s1->getAtom()) && isNumeric(s2->getAtom()))
		{
			if (stringToInt(s1->getAtom().c_str()) < stringToInt(s2->getAtom().c_str())) return new SExp("T");
			else return new SExp("NIL");
		}
		else{ 
			cout << "ERROR: Relational Functions Only Work on Atomic Numeric Nodes!" << endl;
			exit(0);
		}
	}
	else {
		cout << "ERROR: Relational Functions Only Work on Atomic Nodes!"<< endl;
		exit(0);
	}
}

SExp* SExp::isGreater(SExp* s1, SExp* s2) {
	if (isLeaf(s1) && isLeaf(s2))
		if (isNumeric(s1->getAtom()) && isNumeric(s2->getAtom()))
		{
			if (stringToInt(s1->getAtom().c_str()) > stringToInt(s2->getAtom().c_str())) return new SExp("T");
			else return new SExp("NIL");
		}
		else {
			cout << "ERROR: Relational Functions Only Work on Atomic Numeric Nodes!";
			exit(0);
		}
	else { cout <<"ERROR: Relational Functions Only Work on Atomic Nodes!"; exit(0);}
}

SExp* SExp::plus(SExp* s1, SExp* s2) {
	if (isLeaf(s1) && isLeaf(s2))
		if (isNumeric(s1->getAtom()) && isNumeric(s2->getAtom()))
		{
			int sum = stringToInt(s1->getAtom().c_str()) + stringToInt(s2->getAtom().c_str());
			return new SExp(intToString(sum));
		}
		else{
			cout << "ERROR: Arithmetic Functions Only Work on Atomic Numeric Nodes!" << endl;
			exit(0);
		}
	else{
		cout << "ERROR: Arithmetic Functions Only Work on Atomic Nodes!" << endl;
		exit(0);
	}
}

SExp* SExp::minus(SExp* s1, SExp* s2) {
	if (isLeaf(s1) && isLeaf(s2))
		if (isNumeric(s1->getAtom()) && isNumeric(s2->getAtom()))
		{
			int sum = stringToInt(s1->getAtom().c_str()) - stringToInt(s2->getAtom().c_str());
			return new SExp(intToString(sum));
		}
		else {
			cout << "ERROR: Arithmetic Functions Only Work on Atomic Numeric Nodes!" << endl;
			exit(0);
		}
	else {
		cout <<"ERROR: Arithmetic Functions Only Work on Atomic Nodes!" << endl;
		exit(0);
	}
}

SExp* SExp::times(SExp* s1, SExp* s2) {
	if (isLeaf(s1) && isLeaf(s2))
		if (isNumeric(s1->getAtom()) && isNumeric(s2->getAtom()))
		{
			int product = stringToInt(s1->getAtom().c_str()) * stringToInt(s2->getAtom().c_str());
			return new SExp(intToString(product));
		}
		else {cout <<"ERROR: Arithmetic Functions Only Work on Atomic Numeric Nodes!";	exit(0);}

	else{ cout <<"ERROR: Arithmetic Functions Only Work on Atomic Nodes!";exit(0);}
}

SExp* SExp::quotient(SExp* s1, SExp* s2) {
	if (isLeaf(s1) && isLeaf(s2))
		if (isNumeric(s1->getAtom()) && isNumeric(s2->getAtom()))
		{
			int result = stringToInt(s1->getAtom().c_str()) / stringToInt(s2->getAtom().c_str());
			return new SExp(intToString(result));
		}
		else {
			cout <<"ERROR: Arithmetic Functions Only Work on Atomic Numeric Nodes!" << endl;
			exit(0);
		}
	else{
		cout <<"ERROR: Arithmetic Functions Only Work on Atomic Nodes!" << endl;
		exit(0);
	}
}

SExp* SExp::remainder(SExp* s1, SExp* s2) {
	if (isLeaf(s1) && isLeaf(s2))
		if (isNumeric(s1->getAtom()) && isNumeric(s2->getAtom()))
		{
			int dividend = stringToInt(s1->getAtom().c_str());
			int divisor = stringToInt(s2->getAtom().c_str());
			int quotient;
			if (dividend*divisor >= 0)
				quotient = abs(stringToInt(s1->getAtom().c_str())) / abs(stringToInt(s2->getAtom().c_str()));
			else
				quotient = (-1) * abs(stringToInt(s1->getAtom().c_str())) / abs(stringToInt(s2->getAtom().c_str()));
			return new SExp(intToString(dividend-quotient*divisor));
		}
		else {
			cout<<"ERROR: Arithmetic Functions Only Work on Atomic Numeric Nodes!"<<endl;
			exit(0);
		}
	else {
		cout <<"ERROR: Arithmetic Functions Only Work on Atomic Nodes!" << endl;
		exit(0);
	}
}

bool SExp::isLeaf(SExp* s) {
	if (s->getlsexp() == NULL && s->getrsexp() == NULL)
		return true;
	else return false;
}

bool SExp::isNumeric(string literal) {
	if(!isdigit(literal[0])&&literal[0]!='-')
		return false;
	for (int i = 1; i < literal.size(); i++)
	{
		if (!isdigit(literal[i])) return false;
	}
	return true;
}

string SExp::intToString(int value) {
    string s = "";
    bool isNegative = false;
    if (value < 0)
    {
        isNegative = true;
        value *= -1;
    }
    char c;
    do
    {
        c = value % 10 + '0';
        s += c;
        value /= 10;
    } while (value!=0);
    
    int i = 0;
    int j = (int)s.size()-1;
    while (i<j) {
        c=s[i];
        s[i]=s[j];
        s[j]=c;
        i++;
        j--;
    }
    if (isNegative) {
        s='-'+s;
    }
    return s;
}

int SExp::stringToInt(string s) {
    int value = 0;
    bool isNegative = false;
    if (s[0]=='-')
    {
        isNegative = true;
        for (int i = 1; i < s.size(); i++)
        {
            value *= 10;
            value += s[i] - '0';
        }
    }
    else
    {
        for (int i = 0; i < s.size(); i++)
        {
            value *= 10;
            value += s[i] - '0';
        }
    }
    if (isNegative)
    {
        return -1*value;
    }
    else return value;
}