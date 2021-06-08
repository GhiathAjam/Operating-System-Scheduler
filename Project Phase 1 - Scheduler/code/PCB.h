#pragma once
#include <malloc.h>
#include "process_model.h"

enum Process_State
{
STARTED,
STOPED,
RESUMED,
FINISHED
} Process_State;

struct PCB
{
    int id;  //from txt file
    int pid; // real process id
    int arrival_time;
    int run_time;
    int priority;
    int remaining_time; //shared variable
    int waiting_time;
    enum Process_State state;
};


struct PCB * new_pcb(process * p,int pid)
{
    struct PCB *pcb=(struct PCB *)malloc(sizeof(struct PCB));
    pcb->id=p->pid;
    pcb->pid=pid;
    pcb->arrival_time=p->arrival_time;
    pcb->run_time=p->run_time;
    pcb->priority= p->priority;
    pcb->remaining_time=p->run_time;
    pcb->state=-1;
    pcb->waiting_time=0;
    return pcb;
}

int turn_around(struct PCB* pcb,int finished_time)
{
    return (finished_time-(pcb->arrival_time));
}
int wta(struct PCB* pcb,int finished_time)
{
    return (finished_time-(pcb->arrival_time))/(pcb->run_time);
}
//just for the testing
void printpcb(struct PCB* pcb)
{
    printf("id=%d, pid=%d, arrival_time=%d, run_time=%d, priority=%d, remaining_time=%d, waiting_time=%d, state=%d \n", pcb->id,pcb->pid,pcb->arrival_time,pcb->run_time,pcb->priority,pcb->remaining_time,pcb->waiting_time,pcb-> state);
}
