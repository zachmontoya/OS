#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#define SHMSZ 27

int *balance;
sem_t *mutex;

void* MakeTransactions() { // routine for thread execution
	sem_wait(mutex);
 	
	// int shmid;
 	// key_t key = ftok("shmfile", 65);
 	// shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
	// balance = shmat(shmid, NULL, 0);

	int i, j, tmp1, tmp2, rint; double dummy;
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

	sem_post(mutex);
	return NULL; 
} 

int main(int argc, char **argv) { 
	int i; void* voidptr=NULL; pthread_t tid[2]; 
 	srand(getpid());
 	pid_t pid;
 	
	//Shared Memory
 	int shmid;
 	key_t key = ftok("shmfile", 65);
 	shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
 	balance = shmat(shmid, NULL, 0);
 	
	//setting the balance
	balance[0] = balance[1] = 100;
	
	//Setting Mutex
	mutex = sem_open("mutex",O_CREAT,0666,1);
	if(mutex == SEM_FAILED){perror("\nFAILED TO OPEN SEMAPHORE FOR MUTEX\n");exit(-1);}
 	
	//Check int balances
	printf("Init balances A:%d + B:%d ==> %d!\n", balance[0], balance[1], balance[0]+ balance[1]); 

	//FORKING
	pid = fork();

	if (pid == 0) { 	//child process
	printf("CHILD!!\n");
	MakeTransactions();
	}

	else if (pid < 0) {	//error handling
	printf("Error occurred");
	exit(EXIT_FAILURE);
	}
	
	else {			//parent process
	printf("PARENT!!\n");
	MakeTransactions();
	}
 	
	printf("Let's check the balances A:%d + B:%d ==> %d ?= 200\n", balance[0], balance[1], balance[0] + balance[1]); 
	sem_destroy(mutex);
	shmctl(shmid, IPC_RMID, NULL);
 return 0; 
}
