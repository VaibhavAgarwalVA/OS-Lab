#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
 
#define LENARR 1000
 
void main()
{
	char *argv[2];
	argv[0]=(char *)malloc(LENARR*sizeof(char));
	argv[1]=(char *)malloc(LENARR*sizeof(char));
	strcpy(argv[0],"./sort1");
	strcpy(argv[1],"anurag.txt");

	execvp("./sort1",argv);
	perror("Maa chud gayi...\n");
}