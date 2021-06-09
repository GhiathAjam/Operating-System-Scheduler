#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
void minusminus(int signum);
void endprocess(int signum);
int main(int agrc, char *argv[])
{

    signal(SIGUSR1, minusminus);
    signal(SIGINT, endprocess);

    //kill(getpid(), SIGSTOP); //a process should be paused until the schedulder chooses to run it
    //to prevent 2 processes to run together

    initClk();
    remainingtime = atoi(argv[0]);
    //printf("%d", remainingtime);
    printf("I am process :%d  i will start \n", getpid());
    while (remainingtime > 0)
    {
        // busy wait
        // i.e.: running................
        //printf(" i am process %d i am running ...\n",getpid());
    }

    printf("I am process :%d  i will terminte \n", getpid());
    destroyClk(false);
    return 0;
}

void minusminus(int signum)
{
    remainingtime--;
}

void endprocess(int signum)
{
    exit(0);
}
