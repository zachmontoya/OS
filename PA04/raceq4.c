/*=========================================================*/
/* race.c --- for playing with ECE437 */
/*=========================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

sem_t *mutex;
int *Balance;

void* MakeTransactions() { //routine for thread execution   

    sem_wait(mutex); /*MUTEX LOCK!!*/

    /* !!!!!! ENTERING THE CRITICAL SECTION !!!!!! */
    int i, j, tmp1, tmp2, rint; double dummy;
    for (i=0; i < 100; i++) {
        rint = (rand()%30)-15;
        
        if (((tmp1=Balance[0])+rint)>=0 && ((tmp2=Balance[1])-rint)>=0) {      
            Balance[0] = tmp1 + rint; // Adding rint to index 0
            for (j=0; j < rint*1000; j++) {
                dummy=2.345*8.765/1.234;
            } // spend time on purpose
            Balance[1] = tmp2 - rint; // Removing rint from index 1
        }
    }   /* !!!!!! EXITING THE CRITICAL SECTION !!!!!! */
    
    sem_post(mutex);/*MUTEX KEY!!*/    
    
    return NULL;
}

int main(int argc, char **argv) {
    pid_t pid;

    /*Intializing SHM*/
    int shmID;
    key_t key = ftok("shmfile",65);
    shmID = shmget(key,27,IPC_CREAT | 0666);
    Balance = shmat(shmID,NULL,0);
    
    /*Setting Balances*/
    Balance[0] = 100;
    Balance[1] = 100;

    /*Mutex Intialization*/
    mutex = sem_open("mutex",O_CREAT,0666,1);
    if(mutex == SEM_FAILED){perror("\nFAILED TO OPEN SEMAPHORE FOR MUTEX\n");exit(-1);}

    /*Checking Intial Balances*/
    srand(getpid());
    printf("Init balances A:%d + B:%d ==> %d!\n",
    Balance[0],Balance[1],Balance[0]+Balance[1]);
    
    /*Forking*/
    pid = fork();
    if (pid == 0){ // Child
        printf("Hello I am the child\n %d", getpid());
        MakeTransactions();
    }
    else if (pid>0){ // Parent Handler
        printf("Hello I am the parent\n %d", getpid());
        MakeTransactions();
    }
    else{ printf("Error Forking. Fork returned %d",pid); exit(1);} // Fork Error Handling

    /*Checking Balances again after critical section*/
    printf("Let's check the balances A:%d + B:%d ==> %d ?= 200\n",
    Balance[0],Balance[1],Balance[0]+Balance[1]);

    sem_destroy(mutex);
    shmctl(shmID,IPC_RMID,NULL);
    return 0;
}