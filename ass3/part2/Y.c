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

#define P(s) semop(s, &pop, 1)
#define V(s) semop(s, &vop, 1)

int main()
{
	struct sembuf pop,vop;
	int I,J;
	I = semget(2600, 1, 0777|IPC_CREAT);
	J = semget(2599, 1, 0777|IPC_CREAT);

	pop.sem_num = vop.sem_num = 0;
	pop.sem_flg = vop.sem_flg = 0;
	pop.sem_op = -1; 
	vop.sem_op = 1;

	P(J);  // this gets 1 only in X (acts as control gate)
	V(J);  // allow other Y processes to also come inside

	int shmid;
	memo *buff;
	shmid = shmget(2602, 101*sizeof(memo), IPC_CREAT | 0777);
	buff = (memo *) shmat(shmid,NULL,0);

	int shid;
	int *sidebuff;
	shid = shmget(2601, 2*sizeof(int), IPC_CREAT | 0777);
	sidebuff = (int *) shmat(shid,NULL,0);

	int size = sidebuff[0];
	printf("***************MENU-DRIVEN***************\n");
	printf("Enter 1 to SEARCH details by roll number.\n");
	printf("Enter 2 to MODIFY details by roll number.\n");
	printf("Enter 3 to EXIT.\n");
	printf("*****************************************\n");

	do{
		int ch,rno,i,flag;
		float gpa;
		
		printf("\nYour choice...");
		scanf(" %d",&ch);
		
		flag=1;
		switch(ch)
		{
			case 1: //search
						printf("\tEnter roll number for the search candidate... ");
						scanf(" %d",&rno);
						flag=0;
						for(i=0;i<size;i++){
							if(buff[i].roll==rno){
								flag=1;
								printf("\t*****************************************\n");
								printf("\t%s %s %d %f\n",buff[i].firstname, buff[i].lastname, buff[i].roll, buff[i].cgpa);
								printf("\t*****************************************\n");
								break;
							}
						}
						if(flag==0)
							printf("\tNo record found!\n");
						flag=1;
						break;

			case 2: //modify
						printf("\tEnter roll number for the candidate to update... ");
						scanf(" %d",&rno);
						flag=0;
						for(i=0;i<size;i++){
							if(buff[i].roll==rno){
								P(I);
								printf("\t\tRecord found - %s %s! Please enter new GPA: ",buff[i].firstname,buff[i].lastname);
								scanf(" %f",&gpa);
								buff[i].cgpa = gpa;
								printf("\tUpdated!!\n");
								flag=1;
								sidebuff[1] = 1;
								V(I);
								break;
							}
						}
						if(flag==0)
							printf("\tNo record found!\n");
						flag=1;
						break;

			case 3: //exit
						printf("\tAdieu!\n");
						flag=0;
						break;

			default : printf("\tBhag chutiye. Ja khet me tractor chala!!\n");
		}

		if(flag==0)
			break;
	}while(1);

	return 0;
}