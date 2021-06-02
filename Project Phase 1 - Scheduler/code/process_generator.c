#include "headers.h"

void clearResources(int);

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // 1. Read the input files.
    char *input_file_path = argv[1];

    FILE *fptr;
    fptr = fopen(input_file_path, "r");
    if (fptr == NULL)
    {
        printf("Can't Open INPUT File!\n");
        return -1;
    }

    // TODO: STORE THESE IN ARRAY, each will be sent to scheduler at its time
    {
        char *line = NULL;
        int len = 0;
        while (getline(&line, 0, fptr) != -1)
        {
            printf("Retrieved line: %s", line);
            fflush(stdout);
        }
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
    while (1 /* or curr_proc < num of procs*/)
    {
        while (curr_time == getClk())
        {
            //wait
        }
        curr_time++;

        printf("Proc gen, Time: %d\n", curr_time);

        //TODO: implement this: (Sending of "arrived" procs)
        /*
        while (PROCCESES[crr_proc].arrival_time >= curr_time)
            //send curr proc to scheduler
            curr_proc++;
        */
    }

    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption

    //cascade the signal to group
    killpg(getpgrp(), signum);

    exit(0);
}
