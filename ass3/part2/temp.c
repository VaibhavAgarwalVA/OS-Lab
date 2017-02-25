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
	
	int shmid;
	shmid = shmget(2602, 101*sizeof(memo), IPC_CREAT | 0777);
	shmctl(shmid, IPC_RMID, 0);

	int shid;
	shid = shmget(2601, 2*sizeof(int), IPC_CREAT | 0777);
	shmctl(shid, IPC_RMID, 0);

	int I;
	I = semget(2600, 1, 0777|IPC_CREAT);
	semctl(I, 0, IPC_RMID, 0);

	return 0;
}