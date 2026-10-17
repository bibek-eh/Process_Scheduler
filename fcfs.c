/*
    This is a C program to show first come first serve (FCFS) algrithm of CPU scheduler............ 
    */

#include<stdio.h>
#include<conio.h>

#define max 30
void main(){
    int i,j,n,bt[max],wt[max],tat[max];
    float awt=0,atat=0;
    clrscr();
    printf("Enter number of process: ");
    scanf("%d",&n);
    printf("Enter Brust time of the processsor: ");
    for(i=0; i<n; i++){
    scanf("%d",&bt[i]);
    }
    printf("Process\tBrust time\tWaiting time\tTurn around time\n");
    for(i=0; i<n; i++){
        wt[i]=0;
        tat[i]=0;
        for(j=0; j<i; j++){
            wt[i]+=bt[j];
        }
        tat[i]=wt[i]+bt[i];
        awt+=wt[i];
        atat+=tat[i];
        printf("%d\t%d\t\t%d\t\t%d\n",i+1,bt[i],wt[i],tat[i]);
    }
    awt/=n;
    atat/=n;
    printf("Average waiting time: %f\n",awt);  
    printf("Average turn around time: %f\n",atat);  
    getch();
}
