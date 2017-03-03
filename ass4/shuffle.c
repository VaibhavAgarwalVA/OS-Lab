#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#define INTMAX 100

typedef struct tdata
{
	int start;
	int end;
	int type; //0->row, 1->column
	int tid;
	int num;
} tdata;

pthread_mutex_t mutex1;
int myTable[INTMAX][INTMAX];

void prettyprint(int n)
{
	int i,j;
	for(j=0;j<8*n+1;j++){
		if(j%8==0)
			printf("+");
		else
			printf("-");
	}
	printf("\n");
	for(i=0;i<n;i++){
		printf("| ");
		for(j=0;j<n;j++){
			printf("%5d | ",myTable[i][j]);
		}
		printf("\n");
		for(j=0;j<8*n+1;j++){
			if(j%8==0)
				printf("+");
			else
				printf("-");
		}
		printf("\n");
	}
	printf("\n");
}

void start_routine(void *p)
{
	tdata *data = (tdata *) p;
	int st = (*data).start;
	int en = (*data).end;
	int ty = (*data).type;
	int id = (*data).tid;
	int n  = (*data).num;
	int i,j,pivot;

	if(ty==0){
		for(i=st;i<=en;i++){
			pivot = myTable[i][0];
			for(j=0;j<n;j++){
				if(j!=n-1){
					pthread_mutex_lock(&mutex1);
					myTable[i][j]=myTable[i][(j+1)%n];
					pthread_mutex_unlock(&mutex1);
				}
				else{
					pthread_mutex_lock(&mutex1);
					myTable[i][j]=pivot;
					pthread_mutex_unlock(&mutex1);
				}
			}
		}
	}
	else{
		for(i=st;i<=en;i++){
			pivot = myTable[n-1][i];
			for(j=n-1;j>=0;j--){
				if(j!=0){
					pthread_mutex_lock(&mutex1);
					myTable[j][i]=myTable[(j-1)%n][i];
					pthread_mutex_unlock(&mutex1);
				}
				else{
					pthread_mutex_lock(&mutex1);
					myTable[j][i]=pivot;
					pthread_mutex_unlock(&mutex1);
				}
			}
		}
	}

	pthread_exit(NULL);
}

int main()
{
	int i,j,n;
	scanf(" %d",&n);
	
	// //malloc
	// int **myTable;
	// myTable = (int **)malloc(n*sizeof(int *));
	// for(i=0;i<n;i++)
	// 	myTable[i] = (int *)malloc(n*sizeof(int));

	//input
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			scanf(" %d",&myTable[i][j]);
		}
	}

	//printing
	printf("Initial table: \n");
	prettyprint(n);

	//input k,x
	int k,x;
	scanf(" %d %d",&k,&x);

	pthread_t threadid[x];
	pthread_mutex_init(&mutex1,NULL);
	tdata data[x];

	for(i=0;i<k;i++){
		for(j=0;j<x;j++){
			data[j].start = j*(n/x);
			data[j].end = data[j].start + n/x -1;
			if(j==x-1)
				data[j].end = n-1;
			data[j].type = 0;
			data[j].tid = j;
			data[j].num = n;
			pthread_create(&threadid[j],NULL,start_routine,&data[j]);
		}
		for(j=0;j<x;j++){
			pthread_join(threadid[j], NULL);
		}
		printf("After row shift #%d\n",i+1);
		prettyprint(n);
		for(j=0;j<x;j++){
			data[j].start = j*(n/x);
			data[j].end = data[j].start + (n/x) - 1;
			if(j==x-1)
				data[j].end = n-1;
			data[j].type = 1;
			data[j].tid = j;
			data[j].num = n;
			pthread_create(&threadid[j],NULL,start_routine,&data[j]);
		}
		for(j=0;j<x;j++){
			pthread_join(threadid[j], NULL);
		}
		printf("After column shift #%d\n",i+1);
		prettyprint(n);
	}
	
	printf("FINAL TABLE :\n");
	prettyprint(n);

	// //free myTable 
	// for(i=0;i<n;i++)
	// 	free(myTable[i]);
	// free(myTable);

	return 1;
}