#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define INTMAX 1000
#define STRMAX 1000
#define READSIZE 100

int main()
{
	do{
		char str[STRMAX], cwd[STRMAX];
		int status, length; 
		
		getcwd(cwd,100);
		printf("%s > ",cwd);
		scanf("%s",str);

		if(strcmp(str,"cd")==0){
			char cd[STRMAX];
			scanf("%s",cd);
			status = chdir(cd);
			if(status == -1) 
				perror("Error in changing directory!\n");
		}
		else if(strcmp(str,"pwd")==0){
			printf("%s\n",cwd);
		}
		else if(strcmp(str,"mkdir")==0){
			char name[STRMAX];
			scanf("%s",name);
			status = mkdir(name, S_IRWXU | S_IRWXG | S_IRWXO);
			if(status==-1)
				perror("Error in creating new directory!\n");
		}
		else if(strcmp(str,"rmdir")==0){
			char name[STRMAX];
			scanf("%s",name);
			status = rmdir(name);
			if(status==-1)
				perror("Error in removing directory!\n");
		}
		else if(strcmp(str,"ls")==0){
			DIR *dp;
			struct dirent *sd; 
			dp=opendir(cwd);
			while((sd=readdir(dp))!=NULL){
				if(strcmp(sd->d_name,"..")==0 || strcmp(sd->d_name,".")==0)
					continue;
				printf("%s\n",sd->d_name);
			}
			printf("========================\n");
			closedir(dp);
		}
		else if(strcmp(str,"cp")==0){
			printf("cp\n");
		}
		else if(strcmp(str,"exit")==0){
			break;
		}
		else{
			printf("Executable file!\n");
		}
	}while(1);
	return 0;
}