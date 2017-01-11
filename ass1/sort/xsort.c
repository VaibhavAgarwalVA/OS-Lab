#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
 
#define LENARR 1000
 
void main(int argc, char *argv[])
{
	char filename[LENARR];
	strcpy(filename, argv[1]);

	char *params[3];
	params[0]=(char *)malloc(LENARR*sizeof(char));
	params[1]=(char *)malloc(LENARR*sizeof(char));
	strcpy(params[0],"./sort1");
	strcpy(params[1],filename);
	params[2]=NULL;

	execvp("./sort1",params);
	perror("Maa chud gayi...\n");
}