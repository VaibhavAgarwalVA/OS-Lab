#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>
#include <signal.h>

typedef struct memo{
	char firstname[20];
	char lastname[20];
	int roll;
	float cgpa;
} memo;

#define P(s) semop(s, &pop, 1)
#define V(s) semop(s, &vop, 1)

void signalhandler(int signum){

	int shmid;
	memo *buff;
	shmid = shmget(2602, 101*sizeof(memo), IPC_CREAT | 0777);
	buff = (memo *) shmat(shmid,NULL,0);
	shmdt(buff);
	shmctl(shmid, IPC_RMID, 0);

	int shid;
	int *sidebuff;
	shid = shmget(2601, 2*sizeof(int), IPC_CREAT | 0777);
	sidebuff = (int *) shmat(shid,NULL,0);
	shmdt(sidebuff);
	shmctl(shid, IPC_RMID, 0);

	int I;
	I = semget(2600, 1, 0777|IPC_CREAT);
	semctl(I, 0, IPC_RMID, 0);
	
	exit(-1);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, signalhandler);

	char filename[50];
	strcpy(filename,argv[1]);
	
	struct sembuf pop,vop;
	int I,J;
	I = semget(2600, 1, 0777|IPC_CREAT);
	semctl(I, 0, SETVAL, 0);

	pop.sem_num = vop.sem_num = 0;
	pop.sem_flg = vop.sem_flg = 0;
	pop.sem_op = -1; 
	vop.sem_op = 1;

	int shmid;
	memo *buff;
	shmid = shmget(2602, 101*sizeof(memo), IPC_CREAT | 0777);
	buff = (memo *) shmat(shmid,NULL,0);

	int shid;
	int *sidebuff;
	shid = shmget(2601, 2*sizeof(int), IPC_CREAT | 0777);
	sidebuff = (int *) shmat(shid,NULL,0);

	char fname[20], lname[20];
	int rno, i=0, j;
	float cg;
	
	FILE *fp;
	fp = fopen(filename,"r");
	while(fscanf(fp," %s %s %d %f", fname, lname, &rno, &cg)!=EOF){
		strcpy(buff[i].firstname,fname);
		strcpy(buff[i].lastname,lname);
		buff[i].roll = rno;
		buff[i].cgpa = cg;
		i++;
	}
	fclose(fp);
	printf("Fetched %d records!\n",i);

	sidebuff[0] = i;
	sidebuff[1] = 0;

	printf("\n*******************************************\n");
	printf("Shared memory looks like: \n");
	printf("*******************************************\n");
	for(j=0;j<i;j++){
		printf("%s %s %d %f\n", buff[j].firstname, buff[j].lastname, buff[j].roll, buff[j].cgpa);
	}
	printf("*******************************************\n");
	printf("*******************************************\n\n");

	V(I);

	int change = 1;
	do{
		sleep(5);

		change = sidebuff[1];
		if(change){
			sidebuff[1] = 0;
			
			fp = fopen(filename,"w");
			j=0;
			while(j!=i){
				fprintf(fp,"%s %s %d %f\n", buff[j].firstname, buff[j].lastname, buff[j].roll, buff[j].cgpa);
				j++;
			}
			fclose(fp);
			
			printf("*******************************************\n");
			printf("Changes written back to original file!\n");
			printf("*******************************************\n\n");
		}

	}while(1);

	return 0;	
}