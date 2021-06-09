#pragma once
#include <sys/file.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include "process_model.h"

enum Process_State
{
    STARTED,
    STOPED,
    RESUMED,
    FINISHED
} Process_State;

typedef struct PCB
{
    int id;  //from txt file
    int pid; // real process id
    int arrival_time;
    int run_time;
    int priority;
    int remaining_time; //shared variable
    int waiting_time;
    int finish_time;
    enum Process_State state;
}PCB;

struct PCB *new_pcb(process *p, int pid)
{
    struct PCB *pcb = (struct PCB *)malloc(sizeof(struct PCB));
    pcb->id = p->pid;
    pcb->pid = pid;
    pcb->arrival_time = p->arrival_time;
    pcb->run_time = p->run_time;
    pcb->priority = p->priority;
    pcb->remaining_time = p->run_time;
    pcb->state = -1;
    pcb->waiting_time = 0;
    pcb->finish_time = 0;
    return pcb;
}

int turn_around(struct PCB *pcb)
{
    return ((pcb->finish_time) - (pcb->arrival_time));
}
double wta(struct PCB *pcb)
{
    double value = (double)((pcb->finish_time) - (pcb->arrival_time)) / (pcb->run_time);

    int v = (int)(value * 100 + .5);
    double rounded = v / 100.0;

    return rounded;
}
//just for the testing
void printpcb(struct PCB *pcb, int curr_time)
{

    FILE *filePointer;
    if (!(access("scheduler.log", F_OK) == 0))
    {
        filePointer = fopen("scheduler.log", "a+");
        fprintf(filePointer, "#AT time x process y  state arr w total z remain y wait k\n");
    }
    else
        filePointer = fopen("scheduler.log", "a+");

    char state[10];
    switch (pcb->state)
    {
    case STARTED:
        strcpy(state, "started");
        break;
    case STOPED:
        strcpy(state, "stoped");
        break;
    case RESUMED:
        strcpy(state, "resumed");
        break;
    case FINISHED:
        strcpy(state, "finished");
        break;
    }

    fprintf(filePointer, "AT time %d process %d %s arr %d total %d remain %d wait %d", curr_time, pcb->id, state, pcb->arrival_time, pcb->run_time, pcb->remaining_time, pcb->waiting_time);
    if ((pcb->state) == FINISHED)
    {
        fprintf(filePointer, " TA %d  WTA %.2f", turn_around(pcb), wta(pcb));
    }
    fprintf(filePointer, "\n");
    fclose(filePointer);
}
