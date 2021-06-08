#pragma once
#include <malloc.h>
#include "process_model.h"

enum Process_State
{
    WAITNG,
    RUNNING,
    FINISHED,
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
    pcb->state=WAITNG;
    pcb->waiting_time=0;
    return pcb;
}