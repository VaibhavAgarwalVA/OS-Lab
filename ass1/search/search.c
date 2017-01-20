#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define INTMAX 1000
#define STRMAX 1000

int main()
{
	int n,i,j,k,N,id,it,flag;
	int status1,status2;
	int arr[INTMAX];
	char str[STRMAX];

	n=0;
	i=0;
	scanf("%s",str);

	FILE *fp;
	fp = fopen(str,"r");
	while(fscanf(fp,"%d",&arr[i++])!=EOF)
		n++;
	fclose(fp);
	
	do{
		printf("Please enter the search query: ");
		scanf("%d",&k);
		if(k<=0)
			break;
		
		N=n;
		i=0;
		j=n-1;
		status1=0;
		status2=0;
		while(n>10){
			id=fork();
			if(id==0){
				if(status2) 
					exit(1);
				if(status1)
					exit(1);
				i=i;
				j=i+n/2;
				n=n/2+1;
			}
			else{
				wait(&status1);
				if(status1 && N!=n)
					exit(1);
				if(status2 && N!=n)
					exit(1);

				id=fork();
				if(id==0){
					if(status1)
						exit(1);
					if(status2)
						exit(1);
					i=j-(n+1)/2 +1;
					j=j;
					n=(n+1)/2;
				}
				else{
					wait(&status2);
					if(status2 && N!=n)
						exit(1);
					if(status1 && N!=n)
						exit(1);
					if(!status1 && !status2 && N!=n)
						exit(0);
				}
			}

			//printf("%d to %d, %d terms\n",i,j,n);

			if((status1 || status2) && N!=n){
				exit(1);
			}


			if(n<=10){
				flag=0;
				for(it=i;it<=j;it++){
					if(arr[it]==k){
						flag=1;
						if(n!=N)
							exit(1);
					}
				}
				if(n==N && flag){
					status1=1;
					status2=1;
				}
				exit(0);
			}

			if((status1 || status2) && N==n){
				printf("Found!!\n");
				break;
			}

			if(n==N && !status1 && !status2){
				printf("Not found!\n");
				break;
			}
		}
	}while(1);
	printf("Exitted! \n");
}
