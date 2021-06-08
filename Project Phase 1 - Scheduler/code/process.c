#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
void minusminus(int signum);
void proc_pause(int signum);
void proc_resume(int signum);

int main(int agrc, char *argv[])
{
    signal(SIGUSR1,minusminus);
    signal(SIGSTOP,proc_pause);
    signal(SIGUSR2,proc_resume);
    kill(getpid(),SIGSTOP); //a process should be paused until the schedulder chooses to run it 
                            //to prevent 2 processes to run together
    
    initClk();
    remainingtime=atoi(argv[1]);
    

    while (remainingtime > 0)
    {
        // busy wait
        // i.e.: running................
    }

    // hhhhhhhhhhh
    destroyClk(false);
    return 0;
}

void minusminus(int signum)
{
    remainingtime--;
}

void proc_pause(int signum)
{
    //pause untill sent a resume singal
    pause();
}
void proc_resume(int signum)
{

}