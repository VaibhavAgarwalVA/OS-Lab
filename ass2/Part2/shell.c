#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

#define INTMAX 1000
#define STRMAX 1000
#define READSIZE 100

int main()
{
	do{
		char str[STRMAX], cwd[STRMAX], full[STRMAX];
		int status, length, i, l, j; 

		getcwd(cwd,100);
		printf(" %s > ",cwd);
		scanf(" %[^\n]",full);

		//printf("**%s**\t",full);

		l = strlen(full);

		i=0;
		while(str[i])
			str[i++]='\0';

		j=0;
		i=0;
		while(full[i]==' ' && i<l) i++;	
		for(;i<l;i++){
			if(full[i]==' ')
				break;
			str[j++]=full[i];
		}
		while(full[i]==' ' && i<l) i++;

		//printf("**%s**\n",str);

		if(strcmp(str,"cd")==0){
			char cd[STRMAX];
			j=0;
			while(cd[j])
				cd[j++]='\0';
			
			j=0;
			while(full[i]==' ' && i<l) i++;
			for(;i<l;i++)
				cd[j++]=full[i];
			while(full[i]==' ' && i<l) i++;

			status = chdir(cd);
			if(status == -1) 
				perror("Error in changing directory!\n");
		}
		else if(strcmp(str,"pwd")==0){
			printf("%s\n",cwd);
		}
		else if(strcmp(str,"mkdir")==0){
			char name[STRMAX];
			j=0;
			while(name[j])
				name[j++]='\0';

			j=0;
			while(full[i]==' ' && i<l) i++;
			for(;i<l;i++)
				name[j++]=full[i];
			while(full[i]==' ' && i<l) i++;

			status = mkdir(name, S_IRWXU | S_IRWXG | S_IRWXO);
			if(status==-1)
				perror("Error in creating new directory!\n");
		}
		else if(strcmp(str,"rmdir")==0){
			char name[STRMAX];
			j=0;
			while(name[j])
				name[j++]='\0';

			j=0;
			while(full[i]==' ' && i<l) i++;
			for(;i<l;i++)
				name[j++]=full[i];
			while(full[i]==' ' && i<l) i++;
			
			status = rmdir(name);
			if(status==-1)
				perror("Error in removing directory!\n");
		}
		else if(strcmp(str,"ls")==0){
			char extra[STRMAX];
			j=0;
			while(extra[j])
				extra[j++]='\0';

			j=0;
			while(full[i]==' ' && i<l) i++;
			for(;i<l;i++)
				extra[j++]=full[i];
			while(full[i]==' ' && i<l) i++;

			//printf("%s %d\n",extra,j);

			if(j==0){
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
			else{
				//printf("here\n");
				if(strcmp(extra,"-l")==0){
					DIR *direct;
					struct dirent *dirread;
					int fstatus;
					direct=opendir(cwd);
					//printf("here\n");
					if(direct==NULL)
						perror("Empty Directory.");
					else{
						//printf("here\n");
						while((dirread=readdir(direct))!=NULL){
							//printf("herein\n");
							if(strcmp(dirread->d_name,"..")==0 || strcmp(dirread->d_name,".")==0)
								continue;
							struct stat status;
							fstatus=stat(dirread->d_name, &status);
							if(fstatus>=0){
								printf( (S_ISDIR(status.st_mode)) ? "d" : "-");
								printf( (status.st_mode & S_IRUSR) ? "r" : "-");
								printf( (status.st_mode & S_IWUSR) ? "w" : "-");
								printf( (status.st_mode & S_IXUSR) ? "x" : "-");
								printf( (status.st_mode & S_IRGRP) ? "r" : "-");
								printf( (status.st_mode & S_IWGRP) ? "w" : "-");
								printf( (status.st_mode & S_IXGRP) ? "x" : "-");
								printf( (status.st_mode & S_IROTH) ? "r" : "-");
								printf( (status.st_mode & S_IWOTH) ? "w" : "-");
								printf( (status.st_mode & S_IXOTH) ? "x" : "-");
								char *access = ctime(&status.st_mtime);
								int ll = strlen(access);
								access[ll-1]='\0';
								printf(" %5lu  %5lu  %s  %s\n",status.st_nlink,status.st_size,access,dirread->d_name);
							}
						}
					}
					closedir(direct);
				}
				else
					printf("Incorrect command format!\n");
			}
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