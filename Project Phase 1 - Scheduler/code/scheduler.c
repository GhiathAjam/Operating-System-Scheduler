#include "headers.h"
#include "process_model.h"
void clearResources(int);
void initResources();
void recieve_new_processes();
struct linked_list *new_processes;
int arrival_processes_msgq = -1;

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    signal(SIGALRM, recieve_new_processes);

    // puts(argv[0]);

    enum Scheduling_Algorithms algo_num = atoi(argv[0]);
    int quanta = atoi(argv[1]);
    printf("Scheduler started, algo: %d, quanta: %d\n", algo_num, quanta);

    initClk();
    initResources();
    //TODO: implement the scheduler.
    //TODO: upon termination release the clock resources.
    int curr_proc = 0;
    int curr_time = -1;
    printf("Scheduler : All Resources Initialized \n");

    while (1)
    {

        while (curr_time == getClk())
        {
            //wait
        }
        curr_time++;

        //TODO: recieve arrived procs , and make a new PCB for it

        //if no more procs -> exit
        if (((process *)(new_processes->head))->pid == -1)
            break;

        //else: new procs are added, do we really care?

        printf("Scheduler, Time: %d\n", curr_time);

        // A PCB should keep track of the state of a process; running/waiting,
        // execution time, remaining time, waiting time, etc.

        //TODO: 1.Start a new process whenever it arrives. (Fork it and give it its parameters)

        //TODO: check if " curr proc finished " --> check by remaining time --> del its PCB , and send INT to it

        //TODO: Decide if: interrupt currnt proc and start another WHILE SAVING STATE
        //              or continue running curr proc
        //              or schedule next proc if

        // printf("SCHEDULER RUNNING: time is: %d, Running proc: %d\n", curr_time, curr_proc);
        free_linked_list(new_processes); //remove all processes ( They should be added to queue or something before this line)
    }

    //TODO: 5.Report METRICS

    destroyClk(true);

    clearResources
    // after this is terminated -> proc gen will terminate everything
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