#pragma once
#include <sys/file.h>
#include"linked_list.h"

struct process_model
{
    int pid;
    int arrival_time;
    int run_time;
    int priority;

};

#define process  struct process_model

void read_one_process( FILE *fptr , process * proc)
{
    if(!fptr)
        proc=NULL;
    else
    {
        fscanf(fptr,"%d \t %d \t %d \t %d ",&proc->pid,&proc->arrival_time,&proc->run_time,&proc->priority);

    }
}

void read_all_processes(FILE * fptr , struct linked_list * plist)
{
    //TODO: skip lines starting with #
    while(!feof(fptr))
    {
        process * p =malloc(sizeof(process));
        read_one_process(fptr,p);
        struct node * newnode=new_node(p);
        linked_list_push_back(plist,newnode);
    }    
}

