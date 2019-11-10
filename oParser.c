#include "oScanner.c"

int accept(enum sym id);
int isSym(enum sym id);
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

void designator();
void constExpr();
void constDecl();
void expr();

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

    initScanner(fname);

	module();

    closeFile();
}


void module() {
	nextToken();
	accept(sMdul);
	accept(sIdent);
	accept(sSmcln);
	block();
	accept(sDot);
}

void block() {
	declSeq();

	if(isSym(sBgin)) {
		nextToken();
		statSeq();
	}

	accept(sEnd);
	accept(sIdent);
}

void declSeq() {
	if(isSym(sCnst))
	{
		constDecls();
	}

	if(isSym(sType)) {
		typeDecls();
	}

	if(isSym(sVar)) {
		varDecls();
	}

	while(isSym(sProc))
	{
		procDecl();
	}
}

void constDecls() {
	constDecl();
	while(isSym(sIdent))
	{
		constDecl();
	}
}

void constDecl() {
	accept(sIdent);
	accept(sEquls);
	constExpr();
	accept(sSmcln);
}

void typeDecls() {
	typeDecl();
	while(isSym(sIdent))
	{
		typeDecl();
	}
}

void typeDecl() {
	accept(sIdent);
	accept(sEquls);
	type();
	accept(sSmcln);
}

void type() {
	if(isSym(sIdent)) accept(sIdent);
	else if(isSym(sArry)) arrayType();
	else if(isSym(sRcrd)) recordType();
	else if(isSym(sLparen)) enumType();
	else printError("ARRAY, RECORD, or ( not found");
}

void arrayType() {
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
	constExpr();
}

void recordType() {
	accept(sRcrd);
	fieldList();
	while(isSym(sSmcln)) {
		nextToken();
		fieldList();
	}
	accept(sEnd);
}

void enumType() {
	accept(sLparen);
	accept(sIdent);
	while(isSym(sCmma)) {
		nextToken();
		accept(sIdent);
	}
	accept(sRparen);
}

void varDecls() {
	varDecl();
	while(isSym(sIdent)) {
		varDecl();
	}
}

void varDecl() {
	identList();
	accept(sColn);
	type();
	accept(sSmcln);
}

void identList() {
	accept(sIdent);
	while(isSym(sCmma)) {
		nextToken();
		accept(sIdent);
	}
}

void procDecl() {
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
	if(isSym(sVar)){
		nextToken();
	}
	identList();
	accept(sColn);
	formalType();
}

void formalType() {
	accept(sIdent);
}

void procBody() {
	block();
}

void fieldList() {
	if(isSym(sIdent)){
		identList();
		accept(sColn);
		formalType();
	}
}

void statSeq() {
	stat();
	while(isSym(sSmcln)) {
		nextToken();
		stat();
	}
}

void stat() {
}

void assignStat() {
	designator();
	accept(sAsgn);
	expr();
}

void procCall() {
	accept(sIdent);
	
}

void actParams() {
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
	accept(sLparen);
	readParam();
	while(isSym(sCmma)) {
		nextToken();
		readParam();
	}
	accept(sRparen);
}

void readLnParams() {
	readParams();
}

void readParam() {
	designator();
}

void writeParams() {
	accept(sLparen);
	writeParam();
	while(isSym(sCmma)) {
		nextToken();
		writeParam();
	}
	accept(sRparen);
}

void writeLnParams() {
	writeParams();
}

void writeParam() {
	expr();
}

void ifStat() {
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

void designator() {}
void constExpr() {}
void expr() {}

int accept(enum sym id) { 
	int res = 1;
	if(csym.id != id) {
		char exp[255];
		char err[255];
		strcpy(err, csym.value);
		strcpy(exp, sSpell[id]);
		strcat(err, " found, expected ");
		strcat(err, exp);
		printError(err);
		res = 0;
	 }
	 nextToken();
	return res;
}

int isSym(enum sym id) { return csym.id == id; }