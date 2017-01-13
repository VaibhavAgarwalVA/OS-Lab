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
	
	/*
	char *pathvar;
	pathvar = getenv("PATH");
	printf("%s\n",pathvar);

	char newpath[LENARR];
	strcpy(newpath,pathvar);
	strcat(newpath,":/home/");
	setenv("PATH",newpath,1);

	char call[LENARR];
	strcpy(call,"./sort1");
	strcat(call," anurag.txt");*/
	
	execlp("/usr/bin/xterm","/usr/bin/xterm","-hold","-e",params[0],params[1],(char*)NULL);
	perror("Maa chud gayi...\n");
}
