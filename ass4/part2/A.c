	#include <stdio.h>
	#include <sys/types.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
	#include <sys/shm.h>
	#include <signal.h>
	#include <stdlib.h>

	#define in  10
	#define out 11
	#define tot 12

	#define key_queue 1100
	#define key_sem1  1001
	#define key_sem2  1002

	#define P(s); semop(s, &pop, 1)
	#define V(s) semop(s, &vop, 1)



	/**************************functions****************************/ 

	int myRand(int m, int n)   //returns random number between m and n (incl)
	{
		return m+(rand()%(n-m+1));
	}

	/*******/

	int main()
	{
		time_t t;
		srand(time(&t));

		struct sembuf pop,vop;
		int I,J;

		I = semget(key_sem1, 1, 0777);
		while(I<0)
			I = semget(key_sem1, 1, 0777);

		J = semget(key_sem2, 1, 0777);                          // regulation semaphore

		pop.sem_num = vop.sem_num = 0;
		pop.sem_flg = vop.sem_flg = 0;
		pop.sem_op = -1; 
		vop.sem_op = 1;

		printf("Waiting for creation of shared memory!!\n")

		P(I);;
		V(I);

		printf("Shared memory created and initialized!\n");

		int shmid;
		int *buff;
		shmid = shmget(key_queue, 13*sizeof(int), 0777);
		buff = (int *) shmat(shmid,NULL,0);

		while(1){
			int sleeptime = myRand(0,2);
			sleep(sleeptime);

			int num = myRand(-5,5);

			P(J);;
			if(buff[tot]<10){
				buff[buff[in]] = num;
				buff[in]++;
				buff[in]%=10;
				buff[tot]++;
				printf("Adding request : %d\n",num);
			}
			V(J);
		}

		return 0;
	}

