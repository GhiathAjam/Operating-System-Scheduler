#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char *argv[])
{
    initClk();

    //TODO: SCHEDULER WILL SEND TWO SIGNALS WHEN NEEDED TO REPRESENT (PAUSE/RESUME) , WE WILL USE SIGUSR1,SIGUSR2
    //ALSO SIGINT WILL MEAN TERMINATE

    //TODO The process needs to get the remaining time from somewhere
    //remainingtime = ??;
    while (remainingtime > 0)
    {
        // remainingtime = ??;
    }

    destroyClk(false);

    return 0;
}
