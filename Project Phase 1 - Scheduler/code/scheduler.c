#include "headers.h"
#include "process_model.h"
#include "PCB.h"
void clearResources(int);
void initResources();
void recieve_new_processes();

struct PCB *create_new_process(process *); //create  a new process and return it's pcb

void finished_process(struct PCB * pcb); //removes it from ready and add it to finished
void run_process(int pid);
void pause_process(int pid);
void generate_output();

struct linked_list *new_processes;     //list of processes
struct linked_list *ready_processes;   //list of pcbs
struct linked_list *finised_processes; //list of pcbs
                                       //TODO : use this to create output file

struct linked_list *add_processes; //list of pcbs of processes that was added in the current time step
                                   //preemptive algorithms will need it

int arrival_processes_msgq = -1;

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    signal(SIGALRM, recieve_new_processes);

    enum Scheduling_Algorithms algo_num = atoi(argv[0]);
    int quanta = atoi(argv[1]);
    printf("Scheduler started, algo: %d, quanta: %d\n", algo_num, quanta);

    initClk();
    initResources();

    //TODO: implement the scheduler.
    //TODO: upon termination release the clock resources.
    struct PCB *curr_proc = NULL;
    int curr_time = -1;
    new_processes=new_linked_list();     //list of processes
ready_processes=new_linked_list();   //list of pcbs
finised_processes=new_linked_list(); //list of pcbs
    printf("Scheduler : All Resources Initialized \n");
    while (1)
    {
        while (curr_time == getClk())
        {
            //wait
        }
        curr_time++;
        printf("Scheduler, Time: %d\n", curr_time);

        // A PCB should keep track of the state of a process; running/waiting,
        // execution time, remaining time, waiting time, etc.
        struct node *p;
        linked_list_pop_front(new_processes, &p);
        if ( p && ((process *)p->data)->pid == -1)
          break;

        while (p)
        {
            //if no more procs -> exit
            struct PCB *pcb = create_new_process((process *)p->data);
            linked_list_push_front(add_processes, new_node(pcb));
            linked_list_push_front(ready_processes, new_node(pcb));

            linked_list_pop_front(new_processes, &p);
        }

        //else: new procs are added, do we really care?

        //TODO: check if " curr proc finished " --> check by remaining time --> del its PCB

        //TODO: Decide if: interrupt currnt proc and start another WHILE SAVING STATE
        //              or continue running curr proc
        //              or schedule next proc if
        switch (algo_num)
        {
        case FCFS:
            break;
        case SJF:
            break;
        case HPF:
            break;
        case SRTN:
            break;
        case RR:
            break;
        }
    }

    generate_output();

    // after this is terminated -> proc gen will terminate everything
    destroyClk(true);

}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    if (!arrival_processes_msgq == -1) // iniitaized
    {
        printf("Deleting Message Queue\n");
        msgctl(arrival_processes_msgq, IPC_RMID, (struct msqid_ds *)0);
    }
    //cascade the signal to group
    killpg(getpgrp(), signum);

    exit(0);
}

void initResources()
{
    key_t key = ftok("Makefile", 'p');
    arrival_processes_msgq = msgget(key, 0666 | IPC_CREAT);
}
void recieve_new_processes()
{
    int flag = 0;
    while (flag != -1)
    {
        process *np = malloc(sizeof(process));
        flag = msgrcv(arrival_processes_msgq, np, sizeof(process), 0, IPC_NOWAIT);

        if (flag != -1)
        {
            linked_list_push_back(new_processes, new_node(np));
            printf("Scheduler Recieved new process with Id: %d \n", np->pid);
        }
    }
}


struct PCB *create_new_process(process * proc) //create  a new process and return it's pcb
{
    int pid=fork();
    if(pid==0)
    {
        printf("Scheduler forked a new process \n");
        char str_time[2];
        sprintf(str_time, "%d", proc->run_time);
        execl("process.out",str_time,NULL);
    }
    return new_pcb(proc,pid);
}
void finished_process(struct PCB * pcb) //removes it from ready and add it to finished
{
    if(!pcb)return;
    linked_list_remove(ready_processes,pcb);
    pcb->state=FINISHED;
    linked_list_push_front(finised_processes,new_node(pcb));
}
void run_process(int pid)
{
    kill(pid,SIGUSR2);
}
void pause_process(int pid){
    kill(pid,SIGSTOP);
}
void generate_output() {} //TODO implement this function