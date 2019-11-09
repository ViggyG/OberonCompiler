#include "oScanner.c"

int accept(enum sym id);
void module();
void block();

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

	//module();

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
	nextToken();
}


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