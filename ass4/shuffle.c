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
	int tid;
	int num;
	int cnt;
	int xval;
	int times;
} tdata;

pthread_mutex_t mutex1;
pthread_mutex_t cnt_mutex;
pthread_mutex_t cnt_mutex1;
pthread_cond_t cnt_cond;
pthread_cond_t cnt_cond1;
int myTable[INTMAX][INTMAX];
int globalCount,globalCount1;

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
	int id = (*data).tid;
	int n  = (*data).num;
	int ti = (*data).times;
	int x = (*data).xval;

	int i,j,pivot,k;

	//printf("st=%d, en=%d, id=%d, n=%d, ti=%d, x=%d\n",st,en,id,n,ti,x);

	for(k=0;k<ti;k++){
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
		pthread_mutex_lock(&cnt_mutex);
		globalCount++;
		//printf("id=%d,globalCount=%d\n",id,globalCount);
		while(globalCount!=x){
			//printf("waiting id=%d\n",id);
			pthread_cond_wait(&cnt_cond, &cnt_mutex);
			pthread_cond_signal(&cnt_cond);
			break;
		}
		if(globalCount==x){
			//printf("unlock wala banda id=%d\n",id);
			pthread_cond_signal(&cnt_cond);
			globalCount=0;
		}
		pthread_mutex_unlock(&cnt_mutex);
		//printf("bahar id=%d\n",id);

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
		pthread_mutex_lock(&cnt_mutex1);
		globalCount1++;
		//printf("id=%d,globalCount1=%d\n",id,globalCount1);
		while(globalCount1!=x){
			//printf("waiting id=%d\n",id);
			pthread_cond_wait(&cnt_cond1, &cnt_mutex1);
			pthread_cond_signal(&cnt_cond1);
			break;
		}
		if(globalCount1==x){
			//printf("unlock wala banda id=%d\n",id);
			pthread_cond_signal(&cnt_cond1);
			globalCount1=0;
		}
		pthread_mutex_unlock(&cnt_mutex1);
		//printf("bahar id=%d\n",id);
	}
	pthread_exit(NULL);
}

int main()
{
	int i,j,n;
	scanf(" %d",&n);

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
	pthread_mutex_init(&cnt_mutex,NULL);
	pthread_mutex_init(&cnt_mutex1,NULL);
	pthread_cond_init(&cnt_cond, NULL);
	pthread_cond_init(&cnt_cond1, NULL);
	tdata data[x];
	globalCount=0;
	globalCount1=0;

	for(j=0;j<x;j++){
		data[j].start = j*(n/x);
		data[j].end = data[j].start + n/x -1;
		if(j==x-1)
			data[j].end = n-1;
		data[j].tid = j;
		data[j].num = n;
		data[j].xval = x;
		data[j].times = k;
		pthread_create(&threadid[j],NULL,start_routine,&data[j]);
	}
	for(j=0;j<x;j++){
		pthread_join(threadid[j], NULL);
	}
	
	printf("FINAL TABLE :\n");
	prettyprint(n);

	return 1;
}