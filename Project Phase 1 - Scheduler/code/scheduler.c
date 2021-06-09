#include "headers.h"
#include "process_model.h"
#include "PCB.h"
void clearResources(int);
void initResources();
void recieve_new_processes(int);

struct PCB *create_new_process(process *); //create  a new process and return it's pcb

void finished_process(struct PCB *pcb); //removes it from ready and add it to finished
void decreaseTime(struct PCB *curr_proc);
void run_process(int pid);
void pause_process(int pid);
void generate_output(int curr_time, int idle_waiting);

//struct linked_list *new_processes;     //list of processes
struct linked_list *ready_processes;   //list of pcbs
struct linked_list *finised_processes; //list of pcbs
                                       //TODO : use this to create output file

struct linked_list *add_processes; //list of pcbs of processes that was added in the current time step
                                   //preemptive algorithms will need it

int arrival_processes_msgq = -1;

bool recieved = false;
bool recieved_all = false;
//stops current process and starts new process
void contextSwitch(struct PCB *curr_proc, struct PCB *next_proc)
{
    //stop current
    //change it's state
    if (curr_proc)
    {
        pause_process(curr_proc->pid);
        curr_proc->state = STOPED;
    }
    //run next
    //change it's state
    run_process(next_proc->pid);
    next_proc->state = STARTED;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    signal(SIGALRM, recieve_new_processes);

    enum Scheduling_Algorithms algo_num = atoi(argv[0]);
    int quanta = atoi(argv[1]);
    int quanta_counter = 1;
    int idle_waiting = 0;
    struct node *curr_rr_node = NULL;
    printf("Scheduler started, algo: %d, quanta: %d\n", algo_num, quanta);

    initClk();
    initResources();

    //TODO: implement the scheduler.
    //TODO: upon termination release the clock resources.
    struct PCB *curr_proc = NULL;
    int curr_time = 0;
    //  new_processes = new_linked_list();     //list of processes
    ready_processes = new_linked_list();   //list of pcbs
    finised_processes = new_linked_list(); //list of pcbs
    add_processes = new_linked_list();     //list of pcbs
    printf("Scheduler : All Resources Initialized \n");

    while (1)
    {
        while (curr_time == getClk())
        {
            //wait
        }
        curr_time++;
        recieved = false;

        // A PCB should keep track of the state of a process; running/waiting,
        // execution time, remaining time, waiting time, etc.

        //TODO Modify this to make terminate when the process generator sends pid=-1 and the is no more processes in ready queue
        // if (p && ((process *)p->data)->pid == -1)
        //     break;

        //else: new procs are added, do we really care?

        //TODO: check if " curr proc finished " --> check by remaining time --> del its PCB

        //TODO: Decide if: interrupt currnt proc and start another WHILE SAVING STATE
        //              or continue running curr proc
        //              or schedule next proc if

        while (!recieved && !recieved_all)
        {
        }
        printf("Scheduler, Time: %d\n", curr_time);
        printf("Scheduler, Ready: %d\n", ready_processes->count);
        printf("Scheduler, finished: %d\n", finised_processes->count);
        printf("Scheduler, added: %d\n", add_processes->count);

        switch (algo_num)
        {
        case FCFS:
            if (curr_proc)
            {
                decreaseTime(curr_proc);

                if (curr_proc->remaining_time == 0) //finished
                {
                    printf("Scheduler : Process %d finished\n", curr_proc->pid);
                    finished_process(curr_proc);
                    curr_proc = NULL;
                }
            }
            if (!curr_proc && ready_processes->head)
            {
                contextSwitch(curr_proc, ready_processes->head->data);
                curr_proc = ready_processes->head->data;
            }
            break;
        case SJF:
            break;
        case HPF:
            break;
        case SRTN:

            if (curr_proc)
            {
                decreaseTime(curr_proc);

                if (curr_proc->remaining_time == 0) //finished
                {
                    printf("Scheduler : Process %d finished\n", curr_proc->pid);
                    finished_process(curr_proc);
                    curr_proc = NULL;
                }
            }

            struct node *next = NULL;

            //got new processes
            if (add_processes->count > 0)
            {
                printf("There is a new process added \n");
                struct node *ptr = NULL;
                linked_list_pop_front(add_processes, &next);
                ptr = next;
                while (ptr)
                {
                    if (((struct PCB *)ptr->data)->remaining_time < ((struct PCB *)next->data)->remaining_time)
                        next = ptr;
                    linked_list_pop_front(add_processes, &ptr);
                }
            }

            if (next && (!curr_proc || ((struct PCB *)next->data)->remaining_time < curr_proc->remaining_time))
            {
                //context switch

                contextSwitch(curr_proc, (struct PCB *)next->data);
                curr_proc = (struct PCB *)next->data;
            }
            if (!curr_proc && ready_processes->head)
            {
                struct node *next = ready_processes->head;
                struct node *ptr = next->next;
                while (ptr)
                {
                    if (((struct PCB *)ptr->data)->remaining_time < ((struct PCB *)next->data)->remaining_time)
                        next = ptr;
                    ptr = ptr->next;
                }
                contextSwitch(curr_proc, (struct PCB *)next->data);
                curr_proc = (struct PCB *)next->data;
            }
            break;
        case RR:
            if ((ready_processes->count) == 0) //idle waiting
                idle_waiting++;

            if (curr_proc && ((ready_processes->count) > 0))
                decreaseTime(curr_proc); //decrease the process remaining time

            if (!curr_proc && !recieved_all && ((ready_processes->count) > 0)) //initialize
            {
                printf(".................................init\n");
                curr_rr_node = ready_processes->head;
                curr_proc = (struct PCB *)(curr_rr_node->data);
                run_process(curr_proc->pid);                             //run the process
                curr_proc->state = STARTED;                              //set the status of the prosses
                printpcb((struct PCB *)(curr_rr_node->data), curr_time); //print data in schedular.lod
                if (curr_proc->remaining_time == 1)                      //about to finish before it's quanta finish
                {
                    quanta_counter = quanta; //force ti finish next time step
                }
                quanta_counter++; //inc quanta counter
            }
            else if (((ready_processes->count) > 0))
            {
                //check for contex switching
                if ((quanta_counter > quanta) && ((ready_processes->count) > 1)) //means the process has to contex switch and there is another process
                {
                    //first check for the prev process status
                    if (curr_proc->remaining_time == 0) //process finished
                    {
                        printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
                        curr_proc->finish_time = curr_time;
                        curr_proc->waiting_time = (turn_around(curr_proc) - (curr_proc->run_time));
                        printf("Scheduler : Process %d finished\n", curr_proc->pid);
                        curr_rr_node = curr_rr_node->next;
                        if (!curr_rr_node) //round up
                            curr_rr_node = ready_processes->head;
                        finished_process(curr_proc);
                        printpcb(curr_proc, curr_time); //print data in schedular.lod
                    }
                    else //process stoped
                    {
                        printf(".................... %d......\n", curr_proc->id);
                        curr_proc->state = STOPED; //set the status of the prosses
                        pause_process(curr_proc->pid);

                        printpcb(curr_proc, curr_time); //print data in schedular.lod

                        curr_rr_node = curr_rr_node->next;
                        if (!curr_rr_node) //round up
                            curr_rr_node = ready_processes->head;

                        linked_list_remove(ready_processes, curr_proc);
                        linked_list_push_back(ready_processes, new_node(curr_proc));
                    }
                    //run the new process

                    //SET QUANTA COUNTER
                    quanta_counter = 1;

                    curr_proc = (struct PCB *)(curr_rr_node->data);

                    if ((curr_proc->state) == -1) //then it's an idle process
                    {
                        curr_proc->waiting_time = ((curr_time - (curr_proc->arrival_time)) - ((curr_proc->run_time) - (curr_proc->remaining_time)));
                        curr_proc->state = STARTED; //set the status of the prosses
                    }
                    else if ((curr_proc->state) == STOPED)
                    {
                        curr_proc->waiting_time = ((curr_time - (curr_proc->arrival_time)) - ((curr_proc->run_time) - (curr_proc->remaining_time)));
                        curr_proc->state = RESUMED; //set the status of the prosses
                    }
                    printpcb(curr_proc, curr_time);     //print data in schedular.lod
                    if (curr_proc->remaining_time == 1) //about to finish before it's quanta finish
                    {
                        quanta_counter = quanta; //force ti finish next time step
                    }
                    run_process(curr_proc->pid); //run the process
                    //decreaseTime(curr_proc);     //decrease the process remaining time
                    quanta_counter++; //inc quanta counter
                }
                else //check for the curr_proc status of the running process not the quanta
                {

                    if (curr_proc->remaining_time == 0 && ((ready_processes->count) == 1)) //last process finished
                    {
                        curr_proc->finish_time = curr_time;
                        curr_proc->waiting_time = (turn_around(curr_proc) - (curr_proc->run_time));
                        printf("Scheduler : Process %d finished\n", curr_proc->pid);
                        finished_process(curr_proc);
                        printpcb(curr_proc, curr_time); //print data in schedular.lod
                        quanta_counter = 1;
                        curr_proc = NULL;
                    }

                    else if (curr_proc->remaining_time == 1) //about to finish before it's quanta finish
                    {
                        quanta_counter = quanta; //force ti finish next time step
                    }
                    //decreaseTime(curr_proc);
                    quanta_counter++;
                }
            }

            break;
        }
        if (recieved_all && ready_processes->count == 0)
            break;
    }

    generate_output(curr_time, idle_waiting);

    // after this is terminated -> proc gen will terminate everything
    destroyClk(false);
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
void recieve_new_processes(int signum)
{
    if (recieved_all)
        return;
    process *np = malloc(sizeof(process));
    int flag = 0;

    while (flag != -1)
    {

        flag = msgrcv(arrival_processes_msgq, np, sizeof(process), 0, IPC_NOWAIT);
        if (flag != -1)
        {
            printf("Scheduler Recieved new process with Id: %d  , Arrival Time: %d  \n", np->pid, np->arrival_time);
            if (np->pid == -1)
            {
                recieved_all = true;
                break;
            }

            struct PCB *pcb = create_new_process(np);
            //struct node *nde = new_node(pcb);
            // linked_list_push_back(ready_processes, nde);
            // linked_list_push_back(add_processes, nde);
            linked_list_push_back(ready_processes, new_node(pcb));
            linked_list_push_back(add_processes, new_node(pcb));
        }
    }
    recieved = true;
}

struct PCB *create_new_process(process *proc) //create  a new process and return it's pcb
{
    int pid = fork();
    if (pid == 0)
    {
        char str_time[2];
        sprintf(str_time, "%d", proc->run_time);
        execl("process.out", str_time, NULL);
    }

    kill(pid, SIGSTOP);
    printf("Scheduler forked a new process with pid %d \n", pid);
    return new_pcb(proc, pid);
}
void finished_process(struct PCB *pcb) //removes it from ready and add it to finished
{
    if (!pcb)
        return;

    linked_list_remove(ready_processes, pcb);

    pcb->state = FINISHED;
    linked_list_push_front(finised_processes, new_node(pcb));
}
// called every second to decrease  the remainig time
void decreaseTime(struct PCB *curr_proc)
{
    curr_proc->remaining_time--;
    kill(curr_proc->pid, SIGUSR1);
}
// called to run the process with pid
void run_process(int pid)
{
    printf("Scheduler : i will run process %d\n\n", pid);
    kill(pid, SIGCONT);
}
// called to stop the process with pid
void pause_process(int pid)
{
    printf("Scheduler : i will Stop process %d\n\n", pid);
    kill(pid, SIGSTOP);
}

void generate_output(int curr_time, int idle_waiting)
{
    FILE *filePointer = fopen("scheduler.perf", "a+");
    int tot_wait = 0;
    double ulti, tot_wta = 0.0, avg_wait, avg_wta;
    struct node *tt = finised_processes->head;
    struct PCB *pp;
    int count = 0;
    while (tt)
    {
        pp = (struct PCB *)tt->data;
        tot_wait += pp->waiting_time;
        tot_wta += wta(pp);
        count++;
        tt = tt->next;
    }

    double value = (double)((curr_time - idle_waiting) * 100) / curr_time;
    int v = (int)((value * 100) + .5);
    ulti = v / 100.0;

    value = (double)tot_wta / (count);
    v = (int)(value * 100 + .5);
    avg_wta = v / 100.0;

    value = (double)tot_wait / (count);
    v = (int)(value * 100 + .5);
    avg_wait = v / 100.0;

    fprintf(filePointer, "CPU utilization= %.2f %%\nAvg WTA= %.2f\nAvg Waiting=%.2f\n", ulti, avg_wta, avg_wait);
}
