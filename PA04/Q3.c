/*=========================================================*/ 
/* race.c --- for playing with ECE437 */ 
/*=========================================================*/ 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHMSZ 27

int *balance;

void* MakeTransactions() { // routine for thread execution 
 	int shmid;
 	key_t key = ftok("shmfile", 65);
 	shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
	int i, j, tmp1, tmp2, rint; double dummy;
	balance = shmat(shmid, NULL, 0);
	for (i=0; i < 100; i++) { 
		rint = (rand() % 30) - 15; 
 		if (((tmp1 = balance[0]) + rint) >= 0 && ((tmp2 = balance[1]) - rint) >= 0) { 
			balance[0] = tmp1 + rint; 
			for (j=0; j < rint*1000; j++) {
				dummy=2.345*8.765/1.234;
			} // spend time on purpose 
			balance[1] = tmp2 - rint; 
		} 
	} 
	return NULL; 
} 

int main(int argc, char **argv) { 
	int i; void* voidptr=NULL; pthread_t tid[2]; 
 	srand(getpid());
 	pid_t pid;
 	pid = fork();
 	int shmid;
 	key_t key = ftok("shmfile", 65);
 	shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
 	balance = shmat(shmid, NULL, 0);
 	balance[0] = balance[1] = 100;
 	printf("Init balances A:%d + B:%d ==> %d!\n", balance[0], balance[1], balance[0]+ balance[1]); 
	if (pid == 0) { 	//child process
	MakeTransactions();
	}

	else if (pid < 0) {	//error handling
	printf("Error occurred");
	exit(EXIT_FAILURE);
	}
	
	else {			//parent process
	MakeTransactions();
	}
 	printf("Let's check the balances A:%d + B:%d ==> %d ?= 200\n", balance[0], balance[1], balance[0] + balance[1]); 
 return 0; 
}
