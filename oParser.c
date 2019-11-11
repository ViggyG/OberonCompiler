#include "oScanner.c"
#include <stdlib.h>

int accept(enum sym id);
int isSym(enum sym id);
void dPrint(char str[255]);

void module();
void block();
void declSeq();
void constDecls();
void typeDecls();
void typeDecl();
void type();
void arrayType();
void length();
void recordType();
void enumType();
void varDecls();
void varDecl();
void identList();
void procDecl();
void formalParams();
void fPSection();
void formalType();
void procBody();
void fieldList();
void statSeq();
void stat();
void assignStat();
void procCall();
void actParams();
void readParams();
void readLnParams();
void readParam();
void writeParams();
void writeLnParam();
void writeParam();
void ifStat();
void whileStat();
void repeatStat();
void forStat();
void loopStat();
void caseStat();
void pCase();
void caseLabs();
void constExpr();
void expr();
void simplExpr();
void term();
void factor();
void addOp();
void relation();
void mulOp();
void designator();
void selector();
void constDecl();

int debug = 0;
char currentState[255];

int main(int argc, char *argv[]) {

	//csym.id where the symbol is stored after calling nextToken()
	//csym.type is where the spelling of that token s stored
	//csym.value is the original spelling while scanning

	//example: if(csym.id == sMudl) {}

    //retrieving the filename
	char fname[255];
	if(!argv[1])
	{
		//setting filename to and entered value
		printf("Enter File: ");
		scanf("%s", fname);
	}
	else
	{
		//setting filename to command line argument
		strcpy(fname, argv[1]);
	}
	if(argv[2]) debug = 1;

    initScanner(fname);

	module();

    closeFile();
}


void module() {
	dPrint("module");
	nextToken();
	accept(sMdul);
	accept(sIdent);
	accept(sSmcln);
	block();
	accept(sDot);
}

void block() {
	dPrint("block");
	declSeq();

	if(isSym(sBgin)) {
		nextToken();
		statSeq();
	}

	accept(sEnd);
	accept(sIdent);
}

void declSeq() {
	dPrint("declSeq");
	if(isSym(sCnst))
	{
		nextToken();
		constDecls();
	}

	if(isSym(sType)) {
		nextToken();
		typeDecls();
	}

	if(isSym(sVar)) {
		nextToken();
		varDecls();
	}

	while(isSym(sProc))
	{
		procDecl();
	}
}

void constDecls() {
	dPrint("constDecls");
	constDecl();
	while(isSym(sIdent))
	{
		constDecl();
	}
}

void constDecl() {
	dPrint("constDecl");
	accept(sIdent);
	accept(sEquls);
	constExpr();
	accept(sSmcln);
}

void typeDecls() {
	dPrint("typeDecls");
	typeDecl();
	while(isSym(sIdent))
	{
		typeDecl();
	}
}

void typeDecl() {
	dPrint("typeDecl");
	accept(sIdent);
	accept(sEquls);
	type();
	accept(sSmcln);
}

void type() {
	dPrint("type");
	if(isSym(sIdent)) accept(sIdent);
	else if(isSym(sArry)) arrayType();
	else if(isSym(sRcrd)) recordType();
	else if(isSym(sLparen)) enumType();
	else printError("ARRAY, RECORD, or ( not found");
}

void arrayType() {
	dPrint("arrayType");
	accept(sArry);
	length();
	while(isSym(sCmma)) {
		accept(sCmma);
		length();
	}
	accept(sOf);
	type();
}

void length() {
	dPrint("length");
	constExpr();
}

void recordType() {
	dPrint("recordType");
	accept(sRcrd);
	fieldList();
	while(isSym(sSmcln)) {
		nextToken();
		fieldList();
	}
	accept(sEnd);
}

void enumType() {
	dPrint("enumType");
	accept(sLparen);
	accept(sIdent);
	while(isSym(sCmma)) {
		nextToken();
		accept(sIdent);
	}
	accept(sRparen);
}

void varDecls() {
	dPrint("varDecls");
	varDecl();
	while(isSym(sIdent)) {
		varDecl();
	}
}

void varDecl() {
	dPrint("varDecl");
	identList();
	accept(sColn);
	type();
	accept(sSmcln);
}

void identList() {
	dPrint("identList");
	accept(sIdent);
	while(isSym(sCmma)) {
		nextToken();
		accept(sIdent);
	}
}

void procDecl() {
	dPrint("procDecl");
	accept(sProc);
	accept(sIdent);
	if(isSym(sLparen)) {
		formalParams();
	}
	if(isSym(sColn)) {
		nextToken();
		accept(sIdent);
	}
	accept(sSmcln);
	procBody();
	accept(sSmcln);
}

void formalParams() {
	dPrint("formalParams");
	accept(sLparen);
	if(isSym(sVar) || isSym(sIdent)) {
		fPSection();
		while(isSym(sSmcln)){
			nextToken();
			fPSection();
		}
	}
	accept(sRparen);
}

void fPSection() {
	dPrint("fPSection");
	if(isSym(sVar)){
		nextToken();
	}
	identList();
	accept(sColn);
	formalType();
}

void formalType() {
	dPrint("formalType");
	accept(sIdent);
}

void procBody() {
	dPrint("procBody");
	block();
}

void fieldList() {
	dPrint("fieldList");
	if(isSym(sIdent)){
		identList();
		accept(sColn);
		formalType();
	}
}

void statSeq() {
	dPrint("statSeq");
	stat();
	while(isSym(sSmcln)) {
		nextToken();
		stat();
	}
}

void stat() {
	dPrint("stat");
	switch(csym.id) {
		case sIdent:
			assignStat();
			break;
		case sIf:
			ifStat();
			break;
		case sWhl:
			whileStat();
			break;
		case sRep:
			repeatStat();
			break;
		case sFor:
			forStat();
			break;
		case sLoop:
			loopStat();
			break;
		case sCase:
			caseStat();
			break;
		case sExit:
			accept(sExit);
			break;
		case sRet:
			accept(sRet);
			if(isSym(sPlus) || isSym(sMinus) || isSym(sNum) || 
				isSym(sIdent) || isSym(sLparen) || isSym(sTild)) expr();
	}
}

void assignStat() {
	dPrint("assignStat");
	designator();
	accept(sAsgn);
	expr();
}

void procCall() {
	dPrint("procCall");
	accept(sIdent);
	
}

void actParams() {
	dPrint("actParams");
	accept(sLparen);
	if(!isSym(sRparen)) {
		expr();
		while(isSym(sCmma)) {
			nextToken();
			expr();
		}
	}
	accept(sRparen);
}

void readParams() {
	dPrint("readParams");
	accept(sLparen);
	readParam();
	while(isSym(sCmma)) {
		nextToken();
		readParam();
	}
	accept(sRparen);
}

void readLnParams() {
	dPrint("readLnParams");
	readParams();
}

void readParam() {
	dPrint("readParam");
	designator();
}

void writeParams() {
	dPrint("writeParams");
	accept(sLparen);
	writeParam();
	while(isSym(sCmma)) {
		nextToken();
		writeParam();
	}
	accept(sRparen);
}

void writeLnParams() {
	dPrint("writeLnParams");
	writeParams();
}

void writeParam() {
	dPrint("writeParam");
	expr();
}

void ifStat() {
	dPrint("ifStat");
	accept(sIf);
	expr();
	accept(sThn);
	statSeq();
	while(isSym(sElif)) {
		nextToken();
		expr();
		accept(sThn);
		statSeq();
	}
	if(isSym(sEls)) {
		nextToken();
		statSeq();
	}
	accept(sEnd);
}

void whileStat() {
	dPrint("whileStat");
	accept(sWhl);
	expr();
	accept(sDo);
	statSeq();
	while(isSym(sElif)) {
		nextToken();
		expr();
		accept(sDo);
		statSeq();
	}
	accept(sEnd);
}

void repeatStat()
{
	dPrint("repeatStat");
	accept(sRep);
	statSeq();
	accept(sUntl);
	expr();
}

void forStat()
{
	dPrint("forStat");
	accept(sFor);
	accept(sIdent);
	accept(sAsgn);
	expr();
	accept(sTo);
	expr();
	if(isSym(sBy)){
		accept(sBy);
		constExpr();
	}
	accept(sDo);
	statSeq();
	accept(sEnd);
}

void loopStat()
{
	dPrint("loopStat");
	accept(sLoop);
	statSeq();
	accept(sEnd);
}

void caseStat()
{
	dPrint("caseStat");
	accept(sCase);
	expr();
	accept(sOf);
	pCase();
	while(isSym(sBar)) {
		accept(sBar);
		pCase();
	}
	if(isSym(sEls)) {
		accept(sEls);
		statSeq();
	}
	accept(sEnd);
}

void pCase()
{
	dPrint("pCase");
	if(isSym(sPlus) || isSym(sMinus) || isSym(sNum) || 
	isSym(sIdent) || isSym(sLparen) || isSym(sTild))
	{
		caseLabs();
		while(isSym(sCmma)) {
			accept(sCmma);
			caseLabs();
		}
		accept(sColn);
		statSeq();
	}
}

void caseLabs()
{
	dPrint("caseLabs");
	constExpr();
	if(isSym(sDotDot))
	{
		accept(sDotDot);
		constExpr();
	}	
}

void constExpr()
{
	dPrint("constExpr");
	expr();
}

void expr() 
{
	dPrint("expr");
	simplExpr();
	if(isSym(sEquls) || isSym(sHash) || isSym(sLt) || 
	isSym(sLteq) || isSym(sGt) || isSym(sGteq)) {
		relation();
		simplExpr();
	}
}

void simplExpr() 
{
	dPrint("simpleExpr");
	if(isSym(sPlus) || isSym(sMinus))
		nextToken();
	term();
	while(isSym(sPlus) || isSym(sMinus) || isSym(sOr)) {
		addOp();
		term();
	}
}

void term()
{
	dPrint("term");
	factor();
	while(isSym(sAst) || isSym(sDiv) || isSym(sMod) || isSym(sAmp)) {
		mulOp();
		factor();
	}
		
}

void factor()
{
	dPrint("factor");
	switch(csym.id){
		case(sNum):
			accept(sNum);
			break;
		case(sIdent):
			designator();
			if(isSym(sLparen)) actParams();
			break;
		case(sLparen):
			accept(sLparen);
			expr();
			accept(sRparen);
			break;
		case(sTild):
			accept(sTild);
			factor();
			break;
		default: 
			printError("Expected integer, ident, '(', or '~'");
			nextToken();
	}
}

void addOp()
{
	dPrint("addOp");
	if(isSym(sPlus) || isSym(sMinus) || isSym(sOr)) 
		nextToken();
	else printError("Expected '+', '-', or 'OR'");
}

void relation()
{
	dPrint("relation");
	if(isSym(sEquls) || isSym(sHash) || isSym(sLt) || 
	isSym(sLteq) || isSym(sGt) || isSym(sGteq)) 
		nextToken();
	else printError("Expected '=', '#', '<', '<=', '>', or '>='");
}

void mulOp() 
{
	dPrint("mulOp");
	if(isSym(sAst) || isSym(sDiv) || isSym(sMod) || isSym(sAmp))
		nextToken();
	else printError("Expected '*', '&', 'DIV', or 'MOD'");
}

void designator()
{
	dPrint("designator");
	accept(sIdent);
	while(isSym(sDot) || isSym(sLbrac)) {
		selector();
	}
}

void selector() 
{
	dPrint("selector");
	if(isSym(sDot)) {
		accept(sDot);
		accept(sIdent);
	} else {
		accept(sLbrac);
		expr();
		while(isSym(sCmma)) {
    		nextToken();
    		expr();
  		}
		accept(sRbrac);
	}
}

int accept(enum sym id) { 
	int res = 1;
	if(csym.id != id) {
		char exp[255];
		char err[255];
		switch(id) {
			case sIdent:
				strcpy(exp, "sIdent");
				break;
			case sHex:
				strcpy(exp, "sHex");
				break;
			case sNum:
				strcpy(exp, "sNum");
				break;
			default:
				strcpy(exp, sSpell[id]);
				break;
		}
		strcpy(err, csym.value);
		strcat(err, " found, expected ");
		strcat(err, exp);
		printError(err);
		res = 0;
		exit(1);
	 }
	 nextToken();
	return res;
}

void dPrint(char str[255])
{
	if(debug) {
		printf("%s FROM: %s\n",str, currentState);
		strcpy(currentState, str);	
	} 
}

int isSym(enum sym id) { return csym.id == id; }