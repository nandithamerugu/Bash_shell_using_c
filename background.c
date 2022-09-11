#include "cshell.h"

int p = 1;

void background(int start_index, char **token, int last_index)
{
    int to_check = 0;
    char brr[1] = "&"; //for background we use this &
    if(strcmp(token[last_index - 1],"&") == 0)
    {
        to_check = 1;
    }
    
    char **temp;
    if(to_check == 1)
    {
        int i = 0;
        while(i < last_index - 1) // last_index - 1 cause the last index is & operator
        {
            temp[i] = token[i];
            i++;
        }
        temp[i] = NULL;
    }
    else
    {
        int i = 0;
        while(i < last_index)
        {
            temp[i] = token[i];
            i++;
        }
        temp[i] = NULL;
    }

    int pid1;
    pid1 = fork();
    if(pid1 == -1)
    {
        printf("Failed: Forking failed");
    }
    else if(pid1 == 0)
    {
        if(to_check == 1)
        {
            int val1;
            val1 = execvp(token[start_index],temp);
            if(val1 == -1)
            {
                printf("Execution failed %s\n", token[start_index]);
                return;
            }
        }
        else
        {
            int val2;
            val2 = execvp(token[start_index],token);
            if(val2 == -1)
            {
                printf("Execution failed %s\n",token[start_index]);
                return;
            }
        }
        printf("%d",getpgrp());
    }
    else
    {
        if(to_check == 0)
        {
            int pid3;
            int x;
            pid3 = waitpid(pid1,&x, WUNTRACED);
            if(WIFSTOPPED(x))
            {
                printf("%d exited normally\n",pid1);
            }
        }
        else
        {
            bg_pid[p] = pid1;
            strcpy(bg[p],token[0]);
            printf("%d\n",bg_pid[p]);
            p++;
        }
    }  
}
