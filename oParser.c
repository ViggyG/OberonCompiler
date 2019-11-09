#include "oScanner.c"

int main(int argc, char *argv[]) {
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

    closeFile();
}