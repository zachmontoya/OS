#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "random437.h"
#define MAXWAITPEOPLE 800

int CARNUM, MAXPERCAR;
int HOUR = 9;
int MINUTE, SECOND;


// Fill each car
void* fillFord(void* varp){
}

void ride(){
    pthread_t ford[CARNUM];

    // Create threads to fill cars with people
    for(int i = 0; i < CARNUM; i++){
        pthread_create(&ford[i], NULL, fillFord, (void *)&i);
        pthread_join(ford[i], NULL);
    }

    // Display time in HOURS:MINUTES:SECONDS
    // 09:00:00 -
    for(SECOND = 0; SECOND < 7200; SECOND++){
        printf("\n%02d:%02d:%02d", 9 + SECOND / 3600, (SECOND % 3600) / 60, (SECOND % 3600) % 60);
    }
    for(SECOND = 0; SECOND < 7200; SECOND++){
        printf("\n%02d:%02d:%02d", 10 + SECOND / 3600, (SECOND % 3600) / 60, (SECOND % 3600) % 60);
    }
    for(SECOND = 0; SECOND < 7200; SECOND++){
        printf("\n%02d:%02d:%02d", 11 + SECOND / 3600, (SECOND % 3600) / 60, (SECOND % 3600) % 60);
    }
    for(SECOND = 0; SECOND < 7200; SECOND++){
        printf("\n%02d:%02d:%02d", 12 + SECOND / 3600, (SECOND % 3600) / 60, (SECOND % 3600) % 60);
    }
    for(SECOND = 0; SECOND < 7200; SECOND++){
        printf("\n%02d:%02d:%02d", 1 + SECOND / 3600, (SECOND % 3600) / 60, (SECOND % 3600) % 60);
    }
    for(SECOND = 0; SECOND < 7200; SECOND++){
        printf("\n%02d:%02d:%02d", 2 + SECOND / 3600, (SECOND % 3600) / 60, (SECOND % 3600) % 60);
    }
    for(SECOND = 0; SECOND < 7200; SECOND++){
        printf("\n%02d:%02d:%02d", 3 + SECOND / 3600, (SECOND % 3600) / 60, (SECOND % 3600) % 60);
    }
    for(SECOND = 0; SECOND < 7200; SECOND++){
        printf("\n%02d:%02d:%02d", 4 + SECOND / 3600, (SECOND % 3600) / 60, (SECOND % 3600) % 60);
    }

}


int main(int argc, char *argv[]){
    int opt;
    pthread_t carNum;

    // Pass command line arguments
    while ((opt = getopt(argc, argv, "N:" "M:")) != -1){
        switch(opt){
            case 'N':
                CARNUM = atoi(optarg);
                break;
            case 'M':
                MAXPERCAR = atoi(optarg);
                break;
        }
    }

    // Handling error to check for absence of arguments
    if(argc == 1){
        printf("No arguments have been passed!\n");
    }

    // Call the funtion ride to begin the simulation
    ride();

exit (0);
}