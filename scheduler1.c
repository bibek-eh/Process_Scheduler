/*
This program shows actually how the logic of both FCFS & Round Robin works actually its all done using basic concepts of programming of C...
*/

#include<stdio.h>

int FCFS();
int RR();
int work=8,pid=3;


int main(){
    int choice;
    printf("Select the scheduling algorithm:\n");
    printf("1. First Come First Serve (FCFS)\n");
    printf("2. Round Robin (RR)\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);
    switch(choice){
        case 1:
            FCFS();
            break;
        case 2:
            RR();
            break;
        default:
            printf("Invalid choice!\n");
    }
    return 0;
}

int FCFS(){
    printf("You selected First Come First Serve (FCFS) scheduling algorithm.\n");
    for(int i=0;i<pid;i++){
        printf("Process %d is executing...\n",i+1);
        for(int j=0;j<work;j++){
            printf("Process %d working... step %d/%d\n",i+1,j+1,work);
        }
        printf("\nProcess %d finished execution.\n\n",i+1);
    }
    return 0;
}

int RR(){
    int i,j;
    for(i=0; i<3; i++){
        printf("Process %d Started....\n",i+1);
        for(j=1; j<=3; j++){
            printf("Process %d is working... %d/%d\n",i+1,j,work);
        }
        printf("\n");
    }
    for(i=0; i<3; i++){
        printf("Process %d Started....\n",i+1);
        for(j=4; j<=6; j++){
            printf("Process %d is working... %d/%d\n",i+1,j,work);
        }
        printf("\n");
    }
    for(i=0; i<3; i++){
        printf("Process %d Started....\n",i+1);
        for(j=7; j<=8; j++){
            printf("Process %d is working... %d/%d\n",i+1,j,work);
        }
        printf("\n");
    }
    return 0;
}
