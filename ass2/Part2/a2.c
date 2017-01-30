#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(){
	int id, status = 0;
	// creating a child process for sorting
	scanf("%d",&id);
	printf("%d\n",id + 5);	
	return 0;
}
