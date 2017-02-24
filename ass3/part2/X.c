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

int main(int argc, char *argv[])
{
	char filename[50];
	strcpy(filename,argv[1]);

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
	sidebuff[1] = 1;

	printf("\n*******************************************\n");
	printf("Shared memory looks like: \n");
	printf("*******************************************\n");
	for(j=0;j<i;j++){
		printf("%s %s %d %f\n", buff[j].firstname, buff[j].lastname, buff[j].roll, buff[j].cgpa);
	}
	printf("*******************************************\n");
	printf("*******************************************\n\n");


	int change = 1;
	do{
		sleep(5);

		change = sidebuff[1];
		if(change){
			sidebuff[1] = 0;
			
			FILE *fp;
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