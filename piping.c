#include "cshell.h"

// similar to that of what we did for redirecting, we do same for piping also. But we check for '|'.
int to_check_piping(char arr[])
{
    int i = 0;
    int len = strlen(arr);
    while (i < len)
    {
        if (arr[i] == '|') // to check '|'
        {
            return 1;
        }
        i++;
    }
    return 0;
}

void pipe_(char arr[])
{
    int piping[2];
    char *brr[100];
    char *word;
    char *temp[100];
    char *input;
    input = strtok(arr, "|");

    // Till here we will store all the words and seperate based on seperator "|"
    int i;
    for (i = 0; input != NULL; i++)
    {
        temp[i] = input;
        input = strtok(NULL, "|");
    }
    int last_index = i;
    int last = last_index - 1;
    temp[i] = NULL; // Now temp will store all the words

    int fd; // file discriptor
    i = 0;
    for (i = 0; temp[i] != NULL; i++)
    {
        word = strtok(temp[i], " \n\t");
        int j;
        for (j = 0; word != NULL; j++)
        {
            brr[j] = word;
            word = strtok(NULL, " \n\t");
        }
        brr[j] = NULL;
        pipe(piping);
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("Creation of child process is unsuccessful");
        }
        else if (pid == 0) // child is created and returned to it
        {
            dup2(fd, 0); // 0 indicates STDIN;
            if (temp[i + 1] != NULL)
            {
                dup2(piping[1], 1); // 1 indicates STDOUT
            }
            close(piping[0]);
            write_format(brr);
            exit(0);
        }
        else
        {
            // wait(NULL) will block the parent process until any of its children has finished.
            wait(NULL);
            close(piping[1]);
            fd = piping[0];
        }
    }
}