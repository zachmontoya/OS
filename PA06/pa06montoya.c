// ----------------------------------------------------------------------------------
// -- Company: University of New Mexico
// -- Engineer: Issiac Baca & Zachary Montoya, Copyright Univ. of New Mexico
// -- 
// -- Create Date:              12-02-22
// -- Design Name:              PA06BacaMontoya.c
// -- Project Name:             Program Assignment 6
// -- Machine Description:      WSL:Ubuntu 20.04 LTS in Windows 11 OS 
// -- Revision:                 3.3
// -- Additional Comments:
// --                           When compiling on the machine above with gcc the following flags were used: -lm -pthread
// --                           Example: "gcc -o file PA06BacaMontoya.c -lm -pthread && ./file -M 9 -N 6"                                
// -- 
// --                           random437.h was treated as a function and used at the top of this file for portability
// -- 
// ----------------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

#define WaitingLineLength 800

int CARNUM; //Input for NValue.
int MAXPERCAR; //Input for Mvalues
int PoissonResult;
int ParkOpen; // Park status
int GlobalTime;

int CustomersServed;
int TotalWaitTime;
pthread_mutex_t MutexCustomersServed;

int MaxWaitTime;
int MaxWaitTimeOccurrenceEnter;
int MaxWaitTimeOccurrenceExit;
pthread_mutex_t MutexMaxWaitTime;

int GlobalArrival;
int MinuteArrival;
pthread_mutex_t MutexMinuteArrival;

int PeopleOnline;
int WaitingLine;
int count = 0;
int WaitTimeBuffer[WaitingLineLength];
pthread_mutex_t MutexWaitingLine;
sem_t semWaitingLineEmpty;
sem_t semWaitingLineFull;
sem_t binsemWaitLineCounter;

int RejectCount;
pthread_mutex_t MutexRejectCount;

pthread_mutex_t MutexParkOpen;
pthread_cond_t ParkOpenCondVari;
pthread_barrier_t MasterToCarSynchronizationBarrier;

void *CarFunction(void *);
void *GuestFunction(void *);


//Provided Poisson Function random437.h
double U_Random();
int poissonRandom();
int poissonRandom(int meanArrival){
    int k=0;long double p=1.0;
    long double l=exp(-meanArrival);
    double u=U_Random();
    double F=l;
    while(u>=F){   
        k++;
        l*=(double)meanArrival/k;
        F+=l;
    }
    return k;
}
double U_Random(){return (double)rand()/RAND_MAX;}

//Used to format time from integer seconds to HH:MM:SS
void *parkTime(int time, int TimeArray[]){
    TimeArray[0] = (time/3600)+9; //Hours
    int MinIntermediate = time%3600;
    TimeArray[1] = MinIntermediate/60; //Minutes 
    TimeArray[2]= MinIntermediate%60; //Seconds
    TimeArray[3] = time/60;// This is the XXX timestep ranging from 0 to 599;
    return 0; //Used to removed warning. Not sure why this is needed if its a void function
}

/* Master Thread used to synchronize, call guests to line, write status lines, */
void *ParkMaster(){
    int TimeArray[4];
    int a;
    GlobalTime = a;
    sleep(1);
    printf("\nHello I am the Master Thread! \nWelcome to my amusement park! \n\n\nYou're just in time... We are about to open.");
    sleep(1);
    for (int i = 0; i < 10; i++){ // 10 HOURS
        
        for (int j = 0; j < 60; j++){ //60 minutes
            
            pthread_mutex_lock(&MutexMinuteArrival);
            MinuteArrival = 0;
            pthread_mutex_unlock(&MutexMinuteArrival);

            /*Conditional Logic for en-queuing various rates*/
            int z;
            if (a%60 == 0 && 0<=a && a <7200){ //09:00:00--10:59:59, meanArrival = 25 persons per minute
                parkTime(a,TimeArray);
                PoissonResult = poissonRandom(25);
                pthread_t GuestTID[PoissonResult];
                for (z = 0; z  < PoissonResult; z++){        
                    if(pthread_create(&GuestTID[z], NULL, GuestFunction, NULL)!=0){
                        perror("\nUnable to CREATE guest thread in section A");
                    };
                }   
                for (z = 0; z  < PoissonResult; z++){        
                    if(pthread_join(GuestTID[z], NULL)!=0){
                        perror("\nUnable to JOIN guest thread in section A");
                    };
                }
            }              
            else if (a%60 == 0 && 7200<=a && a<18000){//11:00:00--13:59:59, meanArrival = 45 persons per minute
                parkTime(a,TimeArray);
                PoissonResult = poissonRandom(45);
                pthread_t GuestTID[PoissonResult];
                for (z = 0; z  < PoissonResult; z++){        
                    if(pthread_create(&GuestTID[z], NULL, GuestFunction, NULL)!=0){
                        perror("\nUnable to CREATE guest thread in section A");
                    };
                }   
                for (z = 0; z  < PoissonResult; z++){        
                    if(pthread_join(GuestTID[z], NULL)!=0){
                        perror("\nUnable to JOIN guest thread in section A");
                    };
                }
            }
            else if (a%60 == 0 && 18000<=a && a<25200){//14:00:00--15:59:59, meanArrival = 35 persons per minute
                parkTime(a,TimeArray);
                PoissonResult = poissonRandom(35);
                pthread_t GuestTID[PoissonResult];
                for (z = 0; z  < PoissonResult; z++){        
                    if(pthread_create(&GuestTID[z], NULL, GuestFunction, NULL)!=0){
                        perror("\nUnable to CREATE guest thread in section A");
                    };
                }   
                for (z = 0; z  < PoissonResult; z++){        
                    if(pthread_join(GuestTID[z], NULL)!=0){
                        perror("\nUnable to JOIN guest thread in section A");
                    };
                }
            }
            else if (a%60 == 0 && 25200<=a && a<36000){//16:00:00--18:59:59, meanArrival = 25 persons per minute
                parkTime(a,TimeArray);  
                PoissonResult = poissonRandom(25);
                pthread_t GuestTID[PoissonResult];
                for (z = 0; z  < PoissonResult; z++){        
                    if(pthread_create(&GuestTID[z], NULL, GuestFunction, NULL)!=0){
                        perror("\nUnable to CREATE guest thread in section A");
                    };
                }   
                for (z = 0; z  < PoissonResult; z++){        
                    if(pthread_join(GuestTID[z], NULL)!=0){
                        perror("\nUnable to JOIN guest thread in section A");
                    };
                }        
            }

            for (int k = 0; k < 60; k++){ //60Secs
                if(a == 0 ){//Park Opening Instructions
                    for (int y = 3; y > 0; y--){printf("\nOpening in: %d",y);sleep(1);}
                    pthread_mutex_lock(&MutexParkOpen);
                    ParkOpen = 1;
                    printf("\n\nTHE PARK IS OPEN  FOR BUSINESS!!");
                    sleep(2);
                    pthread_cond_broadcast(&ParkOpenCondVari);
                    pthread_mutex_unlock(&MutexParkOpen);

                }
                if(a% 60 == 0){
                    parkTime(a,TimeArray);
                    printf("\nTime Range:%03d   Arrive:%03d   Reject:%05d   Waitline:%03d   at   %02d:%02d:%02d",TimeArray[3],MinuteArrival,RejectCount,WaitingLine,TimeArray[0],TimeArray[1],TimeArray[2]);
                    pthread_barrier_wait(&MasterToCarSynchronizationBarrier);//Used to synchronize cars at the top of the minute
                }
                if (a == 35999) //END OF PARK!
                {
                    PeopleOnline = GlobalArrival - CustomersServed - RejectCount;
                    ParkOpen = 0;
                    a++;
                    GlobalTime = a;
                    parkTime(a,TimeArray);
                    int ResultMaxEnterTimeArray[4];
                    int ResultMaxExitTimeArray[4];
                    parkTime(MaxWaitTimeOccurrenceEnter,ResultMaxEnterTimeArray);
                    parkTime(MaxWaitTimeOccurrenceExit,ResultMaxExitTimeArray);
                    printf("\n                   The Park is Now Closed!\n");
                    printf("\n***************Close of Business (COB) Statistics***************");
                    printf("\nTotal Number of People Arrived:                               %d",GlobalArrival);
                    printf("\nTotal Number of People Still in-line/on the ride at COB:      %d",PeopleOnline);
                    printf("\nTotal Number of People Served                                 %d",CustomersServed);
                    printf("\nTotal Number of People Rejected From Line:                    %d",RejectCount);
                    printf("\n\nAverage Waiting Time                                          %d",TotalWaitTime/CustomersServed);
                    printf("\nThe Maximum Waiting Time was:                                 %d", MaxWaitTime);
                    printf("\nThe Maximum Waiting Time occur at:");
                    printf("\n                      Entered the queue:                      %02d:%02d:%02d",ResultMaxEnterTimeArray[0],ResultMaxEnterTimeArray[1],ResultMaxEnterTimeArray[2]);
                    printf("\n                      Exited the queue:                       %02d:%02d:%02d\n\n",ResultMaxExitTimeArray[0],ResultMaxExitTimeArray[1],ResultMaxExitTimeArray[2]);
                }
                usleep(1);
                a++;
                GlobalTime = a;
            }
        }
    }
}

int main(int argCount, char *argVarible[]){
    //Used to grab -N and -M arguements included when running the executable
    int Nvalue = -1, Mvalue = -1;
    int opt;
    while ((opt = getopt(argCount, argVarible, "N:" "M:")) != -1){
        switch(opt){
            case 'N':Nvalue = atoi(optarg);break;
            case 'M':Mvalue = atoi(optarg);break;
        } 
    }
    MAXPERCAR = Mvalue;
    CARNUM = Nvalue;

    //Intializing pthread and semaphor APIs
    pthread_barrier_init(&MasterToCarSynchronizationBarrier, NULL, CARNUM+1);
    pthread_cond_init(&ParkOpenCondVari,NULL);
    pthread_mutex_init(&MutexParkOpen,NULL);
    pthread_mutex_init(&MutexMinuteArrival,NULL);
    pthread_mutex_init(&MutexCustomersServed,NULL);
    pthread_mutex_init(&MutexMaxWaitTime,NULL);
    sem_init(&semWaitingLineEmpty, 0, WaitingLineLength);
    sem_init(&semWaitingLineFull, 0, 0);
    sem_init(&binsemWaitLineCounter, 0,1);


    //Checking if inputs -N and -M were provided to the executable file
    if (Nvalue != -1 && Mvalue != -1){
        int i;

        pthread_t ParkMasterTID;
        pthread_t FordExplorerTID[CARNUM];

        //Parallel Creation of N+1 Threads. The extra thread is the ParkMaster!!!
        for (i = 0; i < (CARNUM+1); i++){
            if (i == 0){
                if (pthread_create(&ParkMasterTID, NULL, &ParkMaster, NULL) != 0){
                    perror("Failed to create MasterThreadthread");
                    }
            }
            else{
                if (pthread_create(&FordExplorerTID[i], NULL, CarFunction, NULL) != 0){
                    perror("Failed to create FordExplorer thread");
                    }
            }
        }
        for (i = 0; i < (CARNUM+1); i++){
            if (i == 0){
                if (pthread_join(ParkMasterTID, NULL) != 0){
                    perror("Failed to join MasterThread thread");}
            }
            else{
                if (pthread_join(FordExplorerTID[i], NULL) != 0){
                    perror("Failed to join FordExplorer thread");}
            }
        }
        
    }
                
    //Handler if -N and -M values weren't called with the executable file 
    else if (Nvalue == -1 && Mvalue == -1)printf("\n*****\nUser Input Error! Please enter both an N and M value.\n*****\n");

    //Destroying Mutexs and Semaphors
    pthread_barrier_destroy(&MasterToCarSynchronizationBarrier);
    pthread_cond_destroy(&ParkOpenCondVari);
    pthread_mutex_destroy(&MutexMaxWaitTime);
    pthread_mutex_destroy(&MutexCustomersServed);
    pthread_mutex_destroy(&MutexParkOpen);
    pthread_mutex_destroy(&MutexMinuteArrival);
    sem_destroy(&semWaitingLineEmpty);
    sem_destroy(&semWaitingLineFull);
    sem_destroy(&binsemWaitLineCounter);
    exit(10000);   
}

void *CarFunction(void *CarThreadID){
    int *CarID = (int *)CarThreadID;
    while(1){// Loop infinitely
    pthread_mutex_lock(&MutexParkOpen);//Signalling to pause the cars
    if (ParkOpen == 0){pthread_cond_wait(&ParkOpenCondVari, &MutexParkOpen);}
    pthread_mutex_unlock(&MutexParkOpen);

    pthread_barrier_wait(&MasterToCarSynchronizationBarrier);//Used to synchronize the cars at the top of the minute
    // sem_wait(&semWaitingLineFull);

    pthread_mutex_lock(&MutexWaitingLine);
    if (WaitingLine < MAXPERCAR){
        for (int p = 0; p < WaitingLine; p++)
        {
            sem_post(&semWaitingLineEmpty);
            sem_wait(&binsemWaitLineCounter);//Waiting for the guest to document the time before decrementing the buffer counter
            count--;
        }
        CustomersServed = CustomersServed + WaitingLine;
        WaitingLine = WaitingLine - WaitingLine;
        }
    else{
        WaitingLine = WaitingLine - MAXPERCAR;
        CustomersServed = CustomersServed + MAXPERCAR;
        for (int p = 0; p < MAXPERCAR; p++)
        {
            sem_post(&semWaitingLineEmpty);
            sem_wait(&binsemWaitLineCounter);//Waiting for the guest to document the time before decrementing the buffer counter
            count--;
        }
    }
    pthread_mutex_unlock(&MutexWaitingLine);
    
    usleep(100000);
    if (GlobalTime > 35941){return NULL;}
    }
}

void *GuestFunction(void *GuestThreadID){
    int *GuestID = (int *)GuestThreadID;
    
    pthread_mutex_lock(&MutexWaitingLine);
    MinuteArrival++;
    GlobalArrival++;
    pthread_mutex_unlock(&MutexWaitingLine);

    pthread_mutex_lock(&MutexWaitingLine);
    if (WaitingLine >= WaitingLineLength){
        pthread_mutex_lock(&MutexRejectCount);
        RejectCount++;
        pthread_mutex_unlock(&MutexRejectCount);

    }
    pthread_mutex_unlock(&MutexWaitingLine);

    pthread_mutex_lock(&MutexWaitingLine);
    if (WaitingLine < WaitingLineLength){
        
        WaitingLine++;
        WaitTimeBuffer[count] = GlobalTime;
        count++;
        sem_wait(&semWaitingLineEmpty);
        pthread_mutex_lock(&MutexCustomersServed);
        int ThisThreadsWaitDuration = GlobalTime - WaitTimeBuffer[count];
        TotalWaitTime = TotalWaitTime + ThisThreadsWaitDuration;
        if (ThisThreadsWaitDuration>MaxWaitTime){// If the current thread duration is longer than the Max, set new max!
            MaxWaitTime = ThisThreadsWaitDuration;
            MaxWaitTimeOccurrenceEnter = WaitTimeBuffer[count]; // Time it entered the queue 
            MaxWaitTimeOccurrenceExit = GlobalTime; //Time it left waitline
        }
        sem_post(&binsemWaitLineCounter);
        pthread_mutex_unlock(&MutexCustomersServed);
    }
    pthread_mutex_unlock(&MutexWaitingLine);
    return NULL;
}