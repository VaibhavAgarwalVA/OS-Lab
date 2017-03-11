	#include <stdio.h>
	#include <sys/types.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
	#include <sys/shm.h>
	#include <signal.h>
	#include <stdlib.h>

	#define P(s); semop(s, &pop, 1)
	#define V(s) semop(s, &vop, 1)

	#define in  10
	#define out 11
	#define tot 12

	#define key_queue 1100
	#define key_sem1  1001
	#define key_sem2  1002

	int ticket = 100;

	/**************************functions****************************/ 

	int myRand(int m, int n)   //returns random number between m and n (incl)
	{
		return m+(rand()%(n-m+1));
	}

	/*******/

	void book_ticket()
	{
		
	}

	/*******/

	int main()
	{
		time_t t;
		srand(time(&t)); 

		struct sembuf pop,vop;
		int I,J;

		I = semget(key_sem1, 1, 0777|IPC_CREAT);
		semctl(I, 0, SETVAL, 0);

		J = semget(key_sem2, 1, 0777|IPC_CREAT);
		semctl(J, 0, SETVAL, 1);

		pop.sem_num = vop.sem_num = 0;
		pop.sem_flg = vop.sem_flg = 0;
		pop.sem_op = -1; 
		vop.sem_op = 1;

		int shmid;
		int *buff;
		shmid = shmget(key_queue, 13*sizeof(int), IPC_CREAT | 0777);
		buff = (int *) shmat(shmid,NULL,0);
		buff[in]  = 0;
		buff[out] = 0;
		buff[tot] = 0;

		printf("Signal producers to start!\n");

		V(I);

		while(1){
			int req;
			P(J);;
			if(buff[tot]>0){
				req = buff[buff[out]];
				buff[out]++;
				buff[out]%=10;
				buff[tot]--;
				printf("Processing Order : %d\n",req);
			}
			V(J);
		}

		return 0;
	}