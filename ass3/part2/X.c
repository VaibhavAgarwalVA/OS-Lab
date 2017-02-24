#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>

typedef struct memo{
	char firstname[20];
	char lastname[20];
	int roll;
	float cgpa;
} memo;

int main()
{
	int shmid;
	memo *buff;
	shmid = shmget(IPC_PRIVATE, 101*sizeof(memo), IPC_CREAT | 0777);
	buff = (memo *) shmat(shmid,NULL,0);

	do{
		char fname[20], lname[20];
		int rno, i=0, j;
		float cg;
		FILE *fp;
		fp = fopen("dataOS.txt","r");
		while(fscanf(fp," %s %s %d %f", fname, lname, &rno, &cg)!=EOF){
			strcpy(buff[i].firstname,fname);
			strcpy(buff[i].lastname,lname);
			buff[i].roll = rno;
			buff[i].cgpa = cg;
			i++;
		}
		fclose(fp);
		printf("Fetched %d records!\n",i);

		printf("\n*******************************************\n");
		printf("Shared memory looks like: \n");
		printf("*******************************************\n");
		for(j=0;j<i;j++){
			printf("%s %s %d %f\n", buff[j].firstname, buff[j].lastname, buff[j].roll, buff[j].cgpa);
		}
		printf("*******************************************\n");
		printf("*******************************************\n");

		sleep(5);
	}while(1);

	return 0;	
}