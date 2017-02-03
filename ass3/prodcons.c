#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define sum 20
#define in 21
#define out 22

#define P(s) semop(s, &pop, 1)
#define V(s) semop(s, &vop, 1)

int main()
{
	int shmid;
	int *buff;
	shmid = shmget(IPC_PRIVATE, 25*sizeof(int), IPC_CREAT | 0777);
	buff = (int *) shmat(shmid,NULL,0);
	buff[20]=0;
	buff[21]=0;
	buff[22]=0;
	buff[23]=0;

	int m,n;
	printf("Enter 2 integers corresponding to 'm' and 'n' : \n");
	scanf(" %d %d",&m,&n);

	struct sembuf pop,vop;
	int I,J,K,L;

	I = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	J = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	K = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	L = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);

	semctl(I, 0, SETVAL, 20);
	semctl(J, 0, SETVAL, 1);
	semctl(K, 0, SETVAL, 0);
	semctl(L, 0, SETVAL, 1);

	pop.sem_num = vop.sem_num = 0;
	pop.sem_flg = vop.sem_flg = 0;
	pop.sem_op = -1 ; vop.sem_op = 1 ;
	
	int id = fork();
	if(id==0){
		int i,j;
		for(i=0;i<m;i++){
			int id1 = fork();
			if(id1==0){
				for(j=1;j<=50;j++){
					P(J);
					buff[buff[in]]=j;
					buff[in]= (buff[in]+1)%sum;
					printf("writes : %d, in : %d \n",j,buff[in]);
					V(K);
				}
				break;
			}
			else{	
				;	
			}
		}
	}
	else{
		int i,j;
		for(i=0;i<n;i++){
			int id2 = fork();
			if(id2==0){
				int flag=0;
				for(j=0;!flag;j++){
					P(K);
					buff[sum]+= buff[buff[out]];
					buff[out]= (buff[out]+1)%sum;
					printf("sum : %d, out : %d \n",buff[sum],buff[out]);
					if(buff[sum]==m*25*51){
						printf("Obtained sum : %d \n",buff[sum]);
						flag=1;
					}
					V(J);
				}
				printf("Terminated!\n");
				break;
			}
			else{
				;	
			}
		}
		// printf("Terminated!\n");
	}
	return 0;
}