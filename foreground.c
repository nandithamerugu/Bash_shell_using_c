#include "cshell.h"

void foreground(char **arr)
{
    pid_t pid1;
    pid1 = fork(); // same instruction is undertaken and with different pid
    temp.pid = pid1;
    strcat(temp.name, arr[0]);
    int val;
    // printf("%d\n",pid1);
    if (pid1 == -1)
    {
        perror("Error in foreground process!!");
    }
    else if (pid1 == 0)
    {
        // It will make sure that the child process will not get the same process as the parent process
        val = execvp(arr[0], arr);
        if (val < 0)
        {
            perror("Data not found!!!");
        }
    }
    else
    {

        // printf("%d",strcmp(arr[0],"sleep"));
        if (strcmp(arr[0], "sleep") == 0)
        {
            if (arr[1][0] - 48 == 1)
            {
                printf("# sleeps for %d second\n", arr[1][0] - 48);
            }
            else if (arr[1][0] - 48 > 1)
            {
                printf("# sleeps for %d seconds\n", arr[1][0] - 48);
            }
        }
        int storing_parameter;
        // pid1 can be of different values if it is -1 then waiting for child process to end, < -1 then waiting for termination etc.
        waitpid(pid1, &storing_parameter, WUNTRACED);
    }
}