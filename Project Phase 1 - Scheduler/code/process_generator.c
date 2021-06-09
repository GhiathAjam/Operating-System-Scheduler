#include "headers.h"
#include "process_model.h"
void clearResources(int);
void initResources();
struct linked_list *readInputFile(char *);
int scheduler_msgq = -1;
struct linked_list *processes = NULL;

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // 1. Read the input files.
    processes = readInputFile(argv[1]);

    //debug
    struct node *pp = processes->head;
    while (pp)
    {
        process *pr = (process *)pp->data;
        printf("%d\t%d\t%d\t%d\t\n", pr->pid, pr->arrival_time, pr->run_time, pr->priority);
        pp = pp->next;
    }

    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    enum Scheduling_Algorithms algo_num = atoi(argv[2]);
    int quanta = -1;
    if (algo_num == RR)
        quanta = atoi(argv[3]);

    // 3. Initiate and create the scheduler and clock processes.
    int scheduler_pid = fork();
    if (scheduler_pid == 0)
    {
        //converting args to strings
        char str_qua[2];
        sprintf(str_qua, "%d", quanta);
        execl("./scheduler.out", argv[2], str_qua, NULL);
    }

    int clk_pid = fork();
    if (clk_pid == 0)
        execl("./clk.out", NULL);

    //initialize clock.
    initClk();

    // To get time use this function: getClk()
    int curr_proc = 0;
    int curr_time = -1;

    initResources();

    //start simulation
    struct node *next_process = processes->head;
    while (next_process)
    {
        while (curr_time == getClk())
        {
            //wait
        }
        curr_time++;
        //printf("Proc gen, Time: %d\n", curr_time);

        //DONE: implement this: (Sending of "arrived" procs)
        while (next_process && ((process *)next_process->data)->arrival_time == curr_time)
        {
            printf("sending process no. %d at time step=  %d\n", ((process *)next_process->data)->pid, curr_time);
            msgsnd(scheduler_msgq, ((process *)next_process->data), sizeof(process), !IPC_NOWAIT);
            //printf("Proc gen, process with ID: %d has just arrived. \n\n ", ((process *)next_process->data)->pid);
            next_process = next_process->next;
        }
        kill(scheduler_pid, SIGALRM);
    }

    /*
    proc-> data contains:
    struct process_model
    {
        int pid;
        int arrival_time;
        int run_time;
        int priority;
    };
*/

    //tell scheduler to receive

    // Sending empty proc to indicate no more procs are there
    next_process = malloc(sizeof(process));
    ((process *)next_process->data)->pid = -1;
    msgsnd(scheduler_msgq, ((process *)next_process->data), sizeof(process), !IPC_NOWAIT);
    kill(scheduler_pid, SIGALRM);

    //wait for scheduler
    int stat_loc;
    waitpid(scheduler_pid, &stat_loc, 0);

    // 7. Clear clock resources
    destroyClk(true);

    //send TERMINATION signal to EVERYONE
    killpg(getpgrp(), SIGINT);
}

void clearResources(int signum)
{

    free_linked_list(processes);
    //TODO Clears all resources in case of interruption
    if (scheduler_msgq != -1) // iniitaized
    {
        printf("Proc,Gen: Deleting Scheduler Message Queue\n");
        msgctl(scheduler_msgq, IPC_RMID, (struct msqid_ds *)0);
    }

    exit(0);
}

void initResources()
{
    key_t key = ftok("Makefile", 'p');

    scheduler_msgq = msgget(key, 0666 | IPC_CREAT);
}
struct linked_list *readInputFile(char *path)
{
    char *input_file_path = path;

    FILE *fptr;
    fptr = fopen(input_file_path, "r");
    if (fptr == NULL)
    {
        printf("Can't Open INPUT File!\n");
        return NULL;
    }

    /*{
        char *line = NULL;
        int len = 0;
        while (getline(&line, 0, fptr) != -1)
        {
            printf("Retrieved line: %s", line);
            fflush(stdout);
        }
    }
    */

    struct linked_list *processes = new_linked_list();
    read_all_processes(fptr, processes);
    fclose(fptr);
    return processes;
}
