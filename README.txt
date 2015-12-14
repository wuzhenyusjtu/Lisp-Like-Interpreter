Name: Zhenyu Wu
ID: wu.2880

------------Project Directory
/project
 	makefile
	README
	runfile
	/bin
	/src
		*.cpp
    	/obj
	/testcases
		InvalidTests
		ValidTests

------------Instructions on How to compile & test my program
1. How to compile my program?
bash:$ make
2. How to clean in order to recompile?
bash:$ make clean
3. How to run my program?
bash:$ ./Interpreter testfile_name

------------Design
TypeCheck(exp) = 
[	isAtom(exp) then return
	[	isEqual(exp, T) then return BOOL
		isEqual(exp, F) then return BOOL
		isInt(exp) then return NAT
		isEqual(exp, NIL) then return LIST(NAT)
	]
	T	then return
	[	isEqual(car(exp), COND) then 
			return TypeCheckCond(cdr(exp))
		isEqual(car(exp), CAR) then 
			if TypeCheck(cadr(exp))==LIST(NAT) 
				return NAT
		isEqual(car(exp), CDR) then 
			if TypeCheck(cadr(exp))==LIST(NAT) 
				return NAT
		isEqual(car(exp), CONS) then 
			if TypeCheck(cadr(exp))==NAT && TypeCheck(caddr(exp))==LIST(NAT) 
				return LIST(NAT)
		isEqual(car(exp), ATOM) then 
			return BOOL
		isEqual(car(exp), EQ) then
			if TypeCheck(cadr(exp))==NAT && TypeCheck(caddr(exp))==NAT
				return BOOL
		isEqual(car(exp), NULL) then
			if TypeCheck(cadr(exp))==LIST(NAT)
				return BOOL
		isEqual(car(exp), INT) then
			return BOOL
		isEqual(car(exp), PLUS) then
			if Typecheck(cadr(exp))==NAT && TypeCheck(caddr(exp))==NAT
				return NAT
		isEqual(car(exp), LESS) then
			if TypeCheck(cadr(exp))==NAT && TypeCheck(caddr(exp))==NAT
				return BOOL
	]
]

TypeCheckCond(exp) = 
[	isNull(exp) then ERROR
	if TypeCheck(caar(exp)) != BOOL  ERROR
    	type = typeCheck(cadar(s))
	if isNUll(cdr(exp)) then 
		return type
	else checkif type=TypeCheckCond(cdr(exp))
]

Thank you for your grading, Hansey.
Any question please contact me @ 614-721-9374.
