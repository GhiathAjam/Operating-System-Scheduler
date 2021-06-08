#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
void minusminus();

int main(int agrc, char *argv[])
{
    initClk();
    signal(SIGUSR1,minusminus);
    siganl(SIGSTOP,proc_pause);
    siganl(SIGUSR2,proc_resume);

    while (remainingtime > 0)
    {
        // busy wait
        // i.e.: running................
    }

    // hhhhhhhhhhh
    destroyClk(false);
    return 0;
}

void minusminus()
{
    remainingtime--;
}

void proc_pause()
{
    //pause untill sent a resume singal
    pause();
}
void proc_resume()
{

}