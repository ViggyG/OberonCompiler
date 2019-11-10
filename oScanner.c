#include <stdio.h>
#include <string.h>
#include <ctype.h>

//symbol enum for future convienince
enum sym {
	sCnst, sType, sVar, sMdul, sBgin, sProc, sArry, sRcrd,
	sLparen, sRparen, sLBrce, sRBrce, sCmma, sColn, sSmcln,
	sEquls, sLbrac, sRbrac, sPlus, sMinus, sHash, sLt,
	sGt, sAst, sAmp, sDot, sIf, sThn, sElif, sEls, sWhl, sRep,
	sUntl, sDo, sFor, sTo, sBy, sLoop, sCase, sOr, sDiv, sMod, sRet,
	sSqot, sDqot, sSlash, sBar, sAsgn, sGteq, sLteq, sEnd, sDotDot, sOf, sTild, sIdent, sNum, sHex
}; //53

//spelling of tokens for printing
const char * sSpell[] = {
	"sCnst", "sType", "sVar", "sMdul", "sBgin", "sProc", "sArry", "sRcrd",
	"sLparen", "sRparen", "sLBrce", "sRBrce", "sCmma", "sColn", "sSmcln",
	"sEquls", "sLbrac", "sRbrac", "sPlus", "sMinus", "sHash", "sLt",
	"sGt", "sAst", "sAmp", "sDot", "sIf", "sThn", "sElif", "sEls", "sWhl", "sRep",
	"sUntl", "sDo", "sFor", "sTo", "sBy", "sLoop", "sCase", "sOr", "sDiv", "sMod", "sRet",
	"sSqot", "sDqot", "sSlash", "sBar", "sAsgn", "sGteq", "sLteq", "sEnd", "sDotDot", "sOf", "sTild"
};

//values of tokens for comparing
const char * sVals[] = {
	"CONST", "TYPE", "VAR", "MODULE", "BEGIN", "PRODECURE", "ARRAY", "RECORD",
	"(", ")", "{", "}", ",", ":", ";", "=", "[", "]", "+", "-", "#", "<",
	">", "*", "&", ".", "IF", "THEN", "ELSIF", "ELSE", "WHILE", "REPEAT",
	"UNTIL", "DO", "FOR", "TO", "BY", "LOOP", "CASE", "OR", "DIV", "MOD", "RETURN",
	"\'", "\"", "/", "|", ":=", ">=", "<=", "END", "..", "OF", "~"
}; //52

const int SYM_ARR_LENGTH = 54;
const int SPC_LENGTH = 23;

//reserved words
const char * resWords[] = {
	"ABS", "BOOLEAN", "FALSE", "TRUE", "INTEGER",
	"ODD", "READ", "READLN", "WRITE", "WRITELN"
}; //10

//special chars for comparison
char spchrs[] = {
	'(', ')', '{', '}', ',', ':', ';', '=', '[', ']', '|',
	'+', '-', '#', '<', '>', '*', '&', '.', '\'', '\"', '/', '~'
}; //23

//token structure
struct token{
	char value[255];
	char type[255];
	enum sym id;
};

//initial variables
struct token csym;
int isComment = 0;
int isScanning = 0;

FILE * fpointer;
char line[255];
int linePos = 0;

int totalLines = 0;
char tokenBuffer[255] = "";
char * pTokenBuffer = &tokenBuffer[0];
char currentChar;
char nextChar;

//function decl
int nextToken();
void resolveChar(char c, char pc);
void resolveToken();
void resetTokenBuffer();
int chInSet(char set[], char c, int length);
int strInSet(const char * set[], char str[], int length);
void printError(char errstr[]);
void closeFile();

int initScanner(char fname[255])
{	
	//opening file
	fpointer = fopen(fname,"r");
	if(fgets(line, 255, fpointer))
	{
		//load first line
		currentChar = line[linePos++];
		nextChar = line[linePos];
		totalLines++;
	}
	//close file
    return 0;
};

int nextToken()
{
	//init
	int result = 1;
	isScanning = 1;

	//scan loop
	do {	
		//checking the current char is valid
		if(currentChar)
		{
			resolveChar(currentChar, nextChar); //processing char
		}
		else
		{
			//loading a new line if we run out of chars
			if(fgets(line, 255, fpointer)) 
			{
				linePos = 0;
				totalLines++;
			}
			else
			{
				//no more lines so the loop is terminated
				isScanning = 0;
				result = 0;
			} 
		}
		//loading the next char
		currentChar = line[linePos++];
		nextChar = line[linePos];
	} while(isScanning);

	if(result != 0) resolveToken(); //process the new token

	return result; //return 1 if more tokens or 0 if not
};

void resolveChar(char c, char nc)
{
	//init
	int pChar = 1;
	int resToken = 0;

	//checking to see if the char is alphanumeric
	if(isalnum(c))
	{
		//flagging the scanner to stop processing if the next character is a special one
		if(chInSet(spchrs, nc, SPC_LENGTH)) resToken = 1; 
	} 	
	else if (chInSet(spchrs, c, SPC_LENGTH)) //processing special chars
	{ 
		//detecting line comment
		if(c == '/' && nc == '/') 
		{
			isComment = 1;
			resetTokenBuffer();
		} //detecting the other cases where seeing a special char does not mean a new token
		else if(nc == '=' && (c == '<' || c == '>' || c == ':')) resToken = 0;
		else if(c == '.' && nc == '.') resToken = 0;
		else resToken = 1; // creating a new token in all other cases
	}
	
	//creating a new token if the next char is a space but no the current one
	if(isspace(nc) && !isspace(c)) resToken = 1;
	
	//ignoring whitespace characters
	if(isspace(c)) pChar = 0;

	//pushing a new character to the token buffer if flagged
	if(pChar) 
	{
		*pTokenBuffer = c;
		++pTokenBuffer;
		*pTokenBuffer = '\0';
	}
	//stop scanning for a new token
	if(resToken) isScanning = 0;	
}

void resolveToken()
{
	//init
	int result = 1;
	char * tokenValue = tokenBuffer;
	char tokenType[255];

	//finding a token id based on the token
	int tokenID = strInSet(sVals, tokenValue, SYM_ARR_LENGTH);
	
	if(tokenID != -1) //if an id was found get the part of speech
	{
		strcpy(tokenType, sSpell[tokenID]);
	}
	else //token must be an identifier or a number
	{
		//flags and temp vars
		int ind = 0;
		int error = 0;
		char tc;
		//validating a number
		if(isdigit(tokenBuffer[ind])) 
		{
			int ishex = 0; //flag for a hex number
			strcpy(tokenType, "sNum");
			tokenID = (int)sNum;
			
			//validation loop
			while(tc = tokenBuffer[++ind])
			{
				if(isalpha(tc)) 
				{
					if(tc < 'g') 
					{
						ishex = 1;
						tokenID = (int)sHex;
					}
					else error = 1;
				}
				else if(!isdigit(tc)) error = 1;
			}
			//checking to make sure the last char is H
			if(tokenBuffer[ind-1] != 'H' && ishex) error = 1;
			if(error) printError("Invalid Number"); //print error if found
		}
		else if(isalpha(tokenBuffer[ind]))
		{
			strcpy(tokenType, "sIdent");
			tokenID = (int)sIdent;

			//validation loop
			while(tc = tokenBuffer[++ind]) if(!isalnum(tc)) error = 1;

			if(error) printError("Invalid Identifier");//print error if found
		}	
	}

	//setting values on the found token for reference
	strcpy(csym.value, tokenBuffer);
	strcpy(csym.type, tokenType);
	csym.id = tokenID;

	resetTokenBuffer(); //reset the token buffer for the next token
}

//function for reseting the token buffer
void resetTokenBuffer()
{
	pTokenBuffer = &tokenBuffer[0];
	*pTokenBuffer = '\0'; 
}

void closeFile() {
	printf("\n");
	fclose(fpointer);
}

//tool for checking if a character is in a set
int chInSet(char set[], char c, int length)
{
	int result = 0;
	for(int i=0; i<length; i++) if(c == set[i]) result = 1;
	return result;
};

//tool for checking if a string is in a set
int strInSet(const char * set[], char * str, int length)
{
	int position = -1;
	for(int i = 0; i<length; i++) if(strcmp(set[i], str) == 0) position = i;
	return position;
};

//function for printing an error
void printError(char errstr[])
{
	char templine[255];
	printf("ERROR: %s LINE: %d\n", errstr, linePos);
	printf("%s\n",line);
	for(int i=0; i<linePos; i++) printf(" ");
	printf("^\n");
}
