	#include <stdio.h>
	#include <sys/types.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
	#include <sys/shm.h>
	#include <signal.h>
	#include <stdlib.h>
	#include <errno.h>
	#include <unistd.h>
	#include <pthread.h>

	#define P(s); semop(s, &pop, 1)
	#define V(s) semop(s, &vop, 1)

	#define in  10
	#define out 11
	#define tot 12

	#define key_queue 1100
	#define key_sem1  1001
	#define key_sem2  1002

	typedef struct tdata
	{	
		int x;
	} tdata;


	/**************************declarations****************************/ 

	int ticket  = 100;
	int threads = 0;

	int shmid;
	int *buff;
	struct sembuf pop,vop;
	int I,J;
	pthread_mutex_t mutex1;


	/**************************functions****************************/ 	

	int myRand(int m, int n)   //returns random number between m and n (incl)
	{
		return m+(rand()%(n-m+1));
	}

	/*******/

	void signalhandler(int signum)
	{
		shmdt(buff);
		shmctl(shmid,IPC_RMID,0);
		semctl(I, 0, IPC_RMID, 0);
		semctl(J, 0, IPC_RMID, 0);

		exit(-1);
	}

	void book_ticket(void *p)
	{
		tdata *data = (tdata *) p;
		int req = (*data).x;

		if((ticket-req)>=0){
			pthread_mutex_lock(&mutex1);
			ticket = (ticket-req) > 100 ? 100 : (ticket-req);
			pthread_mutex_unlock(&mutex1);
			int sleeptime = myRand(0,2);
			sleep(sleeptime);
			printf("Ticket : %d\n",ticket);
			threads--;
			pthread_exit((void *)1);
		}
		else{
			int sleeptime = myRand(0,2);
			sleep(sleeptime);
			printf("Request can't be fulfilled!\n");
			threads--;
			pthread_exit((void *)0);
		}
	}

	/*******/

	int main()
	{
		signal(SIGINT,signalhandler);

		time_t t;
		srand(time(&t)); 


		I = semget(key_sem1, 1, 0777|IPC_CREAT);
		semctl(I, 0, SETVAL, 0);

		J = semget(key_sem2, 1, 0777|IPC_CREAT);
		semctl(J, 0, SETVAL, 1);

		pop.sem_num = vop.sem_num = 0;
		pop.sem_flg = vop.sem_flg = 0;
		pop.sem_op = -1; 
		vop.sem_op = 1;

		shmid = shmget(key_queue, 13*sizeof(int), IPC_CREAT | 0777);
		buff = (int *) shmat(shmid,NULL,0);
		buff[in]  = 0;
		buff[out] = 0;
		buff[tot] = 0;

		// printf("Signal producers to start!\n");

		V(I);

		tdata data[10];
		pthread_t threadid[10];	
		pthread_mutex_init(&mutex1,NULL);

		while(1){
			int req,i;
			i=-1;

			P(J);;
			if(buff[tot]>0){
				req = buff[buff[out]];
				i = buff[out];
				buff[out]++;
				buff[out]%=10;
				buff[tot]--;
				printf("Processing Order : %d\n",req);
			}
			V(J);

			if(i>=0){
				data[i].x = req;
				pthread_create(&threadid[i],NULL,book_ticket,&data[i]);
				threads++;
			}

			if(threads==10){
				while(threads>5);
			}

		}

		return 0;
	}