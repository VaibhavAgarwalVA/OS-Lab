#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define INTMAX 1000
#define STRMAX 1000
#define READSIZE 100

int main()
{
	int i,id;
	id=fork();
	if(id==0){
		printf("Welcome to Vaibhav's Linux Shell!\n");
		char *argv[2];
		argv[0]=(char *)malloc(STRMAX*sizeof(char));
		strcpy(argv[0],"./shell");
		argv[1]=NULL;

		execvp(argv[0],argv);
		perror("Shell startup error!\n");
	}
	else{
		wait(NULL);
		printf("Exitting shell!\n");
	}
	return 0;
}