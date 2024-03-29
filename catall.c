#include <string.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[])
{
	char *cat="/bin/cat";

	if(argc<2){
	printf("Ingrese la ruta.\n");
	return 1;
	}
	
	char *command = malloc(strlen(cat) + strlen(argv[1])+2);
	printf(command, "%s %s", cat, argv[1]);
	system(command);
	return 0;
}
