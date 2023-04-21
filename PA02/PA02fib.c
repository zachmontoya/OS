#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#define SHMSZ 128

int Result;

// CASE2 - slow/recursive implementation of Fib
int case2(int x) { 
    int i, rint = (rand()%30);
    double dummy;
    for (i=0; i<rint*100; i++)dummy=2.345*i*8.765/1.234;
    if (x==0)return(0);
    else if (x==1)return(1);
    else return(case2(x-1)+case2(x-2));
}

//CASE1 - Work in progress
int forkfib(int n){
    
    //Setup for Resultant 
    int *ResArray;
    int ResSMID;
    key_t Reskey = ftok("ResSharedMemorFib3",65); //Grabbing Key
    ResSMID = shmget(Reskey, SHMSZ, IPC_CREAT | 0666); // Creating Shared Memory Location
    ResArray = (int *)shmat(ResSMID, 0, 0); // Attaching
    //Setup for Result Indexer
    int *IndexerRes;
    int IndexerResSMID;
    key_t Indexerreskey = ftok("IndexerResSharedMemoryFib3",65); //Grabbing Key
    IndexerResSMID = shmget(Indexerreskey, SHMSZ, IPC_CREAT | 0666); // Creating Shared Memory Location
    IndexerRes = (int *)shmat(IndexerResSMID, 0, 0); // Attaching
    //Setup for Adder Indexer
    int *IndexerArray;
    int IndexerSMID;
    key_t Indexerkey = ftok("IndexerSharedMemoryFib3",65); //Grabbing Key
    IndexerSMID = shmget(Indexerkey, SHMSZ, IPC_CREAT | 0666); // Creating Shared Memory Location
    IndexerArray = (int *)shmat(IndexerSMID, 0, 0); // Attaching
    //Setup for Adder Array
    int shmid;
    int *array;
    key_t key = ftok("ArraySharedMemoryFib3",65); //Grabbing Key
    shmid = shmget(key, SHMSZ, IPC_CREAT | 0666); // Creating Shared Memory Location
    array = (int *)shmat(shmid, 0, 0); // Attaching

    if(n == 0 || n == 1){
        array[IndexerArray[0]] = n;
        IndexerArray[0] = IndexerArray[0] + 1;
        return(0);
    }
    
    else{
        pid_t ChildPid = fork();
        if(ChildPid == 0){ //child
            forkfib(n-1);
            exit(0); 
        }

        else{ //parent
            waitpid(ChildPid,NULL,0);
            forkfib(n-2);
            int ChildIndex = IndexerArray[0];
            ChildIndex = ChildIndex - 2;
            int ParentIndex = IndexerArray[0];
            ParentIndex = ParentIndex - 1;
            int childResult = array[ChildIndex];
            int parentResult = array[ParentIndex];
            int Fibn = childResult + parentResult;
            ResArray[100] =  ResArray[100] + Fibn; 
            // Removing Memory Location
            int rmidIndexer = shmctl(IndexerSMID, IPC_RMID, NULL); 
            int rmidArray = shmctl(shmid, IPC_RMID, NULL); 
            int rmidResArray = shmctl(ResSMID, IPC_RMID,NULL);
            int rmidResIndexer = shmctl(IndexerResSMID, IPC_RMID,NULL);
            return(ResArray[100]); 
        }
    }
}

int main(int argCount, char *argVarible[]){
    int Fvalue, Svalue;
    int opt;
    int x = 0;
    while ((opt = getopt(argCount, argVarible, "F:" "S:")) != -1){
        switch(opt){
			case 'F':
                Fvalue = atoi(optarg);
                break;
			case 'S':
                Svalue = atoi(optarg);
                break;
		}
    }

    //CASE 1 CODE
    if((Fvalue-1)>Svalue||(Fvalue-2)>Svalue){
        Result = forkfib(Fvalue);
    }
    //CASE 2 CODE
    else if ((Fvalue-1)<=Svalue&&(Fvalue-2)<=Svalue){
        Result = case2(Fvalue);
    }

    printf(" \n\n%d\n\n", Result);
    
    exit (0);
}
