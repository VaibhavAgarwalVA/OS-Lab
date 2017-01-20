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
	
	int arr[LENARR],n,i,j,temp;
	n=0;
	
	FILE *fp;
	fp = fopen(filename,"r");
	while(fscanf(fp,"%d",&arr[i++])!=EOF)
		n++;
	fclose(fp);
	
	//bubble sort
	for(i=0;i<n-1;i++){
		for(j=0;j<n-i-1;j++){
			if(arr[j]>arr[j+1]){
				temp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
			}
		}
	}

	printf("Sorted list in ascending order: ");

	for(i=0;i<n;i++)
		printf("%d ",arr[i]);
		
	printf("\n");	
}
