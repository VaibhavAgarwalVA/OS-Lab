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

int main(int argc, char *argv[])
{
	char file1[STRMAX];
	char file2[STRMAX];
	strcpy(file1,argv[1]);
	strcpy(file2,argv[2]);
	
	int file1des = open(file1,O_RDONLY);
	if(file1des<0){
		perror("File 1 read error!\n");
		return 0;
	}

	int file2des = open(file2, O_TRUNC | O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR);
	if(file2des<0){
		perror("File 2 IO error!\n");
		return 0;
	}	

	int fd[2],readbytes, ret[2], retbytes;    //rd -> pipe1 and ret -> pipe2
	pipe(fd);
	pipe(ret);
	
	int id = fork();
	if(id!=0){
		close(fd[0]);  //close read for parent process
		close(ret[1]); //close write for parent process

		char buf[READSIZE];
		char arr[1];
		read(ret[0],arr,1);
		//printf("Parent is ready for next read/write!\n");
		
		while(readbytes=read(file1des,buf,READSIZE)){
			write(fd[1],buf,readbytes);
			//printf("Parent has read from file1 and written something on buffer!\n");
			read(ret[0],arr,1);
			if(arr[0]=='-1'){
				printf("Terminated due to error!\n");
				return 0;
			}
			if(arr[0]=='1'){
				printf("Successfully completed copy!!\n");
				return 0;
			}
			//printf("Parent is ready for next read/write!\n");
		}
	}
	else{
		close(fd[1]);  //close write for child process
		close(ret[0]); //close read for child process

		char buf[READSIZE];
		write(ret[1],"0",1);
		//printf("Child acknowledges..\n");
		char arr[1];

		while(readbytes=read(fd[0],buf,READSIZE)){
			strcpy(arr,"0");
			if(readbytes<=0){
				perror("Error in child from reading from Pipe1\n");
				strcpy(arr,"-1");
			}
			int signal = write(file2des,buf,readbytes);
			if(signal==readbytes && readbytes<100){
				printf("File copied successfully!!\n");
				strcpy(arr,"1");
			}
			if(signal!=readbytes){
				perror("Error in writing to external file File2 in child!\n");
				strcpy(arr,"-1");
			}

			write(ret[1],arr,1);
			//printf("Child acknowledges..\n");
			if(arr[0]=='-1' || arr[0]=='1')
				exit(1);
		}
	}
	close(file2des);
	close(file1des);
	return 0;
}
