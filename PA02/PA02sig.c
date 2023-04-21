#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//Global Variables
pid_t ChildPIDValue; 
int ToggleBit = 0;

void userDef_signalHandlerC(int sigC){//CTRL+C is to terminate child and exit the parent.
    printf("\nCAUGHT SIGNAL CTRLC!!");
    if(ChildPIDValue>0){
        printf("\nKilling the Child Process and about to exit the Parent Process.\n");
        kill(ChildPIDValue,SIGKILL);
        exit(0);
    }
}

void userDef_signalHandlerZ(int sigZ){//CTRL+Z is the toggle INT
    printf("\n\nCAUGHT SIGNAL CTRLZ!!");
    if(ChildPIDValue>0){
        if(ToggleBit<1){//CHILD IS RUNNING IF TOGGLEBIT==0
            printf("\nABOUT TO PAUSE PID:%d",ChildPIDValue);
            kill(ChildPIDValue,SIGSTOP);
            ToggleBit = 1;
            printf("\n****PAUSE COMPLETE****\n");
        } 
        else{//CHILD IS STOPPED IF TOGGLEBIT==1
            printf("\nABOUT TO RESUME PID:%d",ChildPIDValue);
            kill(ChildPIDValue,SIGCONT);
            ToggleBit = 0;    
            printf("\n****RESUME COMPLETE****\n");
        }
    }
}

int main(){
    ChildPIDValue = fork();//FORK  
    if (ChildPIDValue > 0){//Parent Code
        //CTRL+C sigaction setup - STOP & EXIT
        struct sigaction act;
        act.sa_handler = userDef_signalHandlerC;
        sigemptyset(&act.sa_mask);
        sigaddset(&act.sa_mask, SIGINT);
        act.sa_flags=SA_RESTART;
        sigaction(SIGINT, &act, NULL);
        //CTRL+Z sigaction setup - TOGGLE
        struct sigaction act_2;
        act_2.sa_handler = userDef_signalHandlerZ;
        sigemptyset(&act_2.sa_mask);
        sigaddset(&act_2.sa_mask, SIGTSTP);
        act.sa_flags = SA_RESTART;
        sigaction(SIGTSTP, &act_2, NULL);
        while(1){}
        return 0;
    }
    else if(ChildPIDValue == 0){
        //Child Code
        printf("Hello! I am the child process and i recieved: %d",ChildPIDValue);
        char *args[] = {};
        char *env[] = {}; 
        execve("/bin/yes",args,env); 
    }
    else perror("\n!!!!!!!\nFork Error");
    return 0 ;
}