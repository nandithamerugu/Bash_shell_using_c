#include "cshell.h"

void both_redirection_piping(char arr[], char *inp)
{
    int piping[2];
    char *brr[100];
    char *word;
    char *temp[100];
    char *input;
    char name_of_file[100];
    int fd;
    input = strtok(arr, "|");

    // Till here we will store all the words and seperate based on seperator "|"
    int i;
    for (i = 0; input != NULL; i++)
    {
        temp[i] = input;
        input = strtok(NULL, "|"); // ls | grep *.txt > out.txt say this is the example, using this loop we get the two strings into temp i.e., ls and grep *.txt > out.txt
    }
    int last_index = i;
    int last = last_index - 1;
    temp[i] = NULL; // Now temp will store all the words
    int val;
    dup2(STDOUT_FILENO, val);
    int val_type;
    for (i = 0; temp[i] != NULL; i++)
    {
        val_type = to_check_type(temp[i]);
        word = strtok(temp[i], " < >> >\n\t"); // using the given loops we furhur segregate them into many other strings i.e., grep *.txt, out.txt, ls
        int j;
        for (j = 0; word != NULL; j++)
        {
            brr[j] = word;
            word = strtok(NULL, " < >> >\n\t"); // making the word null
        }
        brr[j] = NULL;
        int length = j - 1;

        if (val_type == 2 || val_type == 3) // if it has only append or overwrite
        {
            strcpy(name_of_file, brr[length]);
            brr[length] = NULL;
        }
        pipe(piping);
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Creation of child process is unsuccessful");
        }
        else if (pid == 0)
        {
            // child is created and returned to it
            dup2(fd, 0); // 0 indicates STDIN;

            if (temp[i + 1] != NULL)
            {
                dup2(piping[1], 1); // piping[1] will be used to read whereas piping[0] to write
            }
            else
            {
                if (val_type == 2)
                {
                    fd = open(name_of_file, O_APPEND | O_CREAT | O_WRONLY, 0644);
                    dup2(fd, STDOUT_FILENO);
                }
                else if (val_type == 3)
                {
                    fd = open(name_of_file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
                    dup2(fd, STDOUT_FILENO);
                }
                // switch(val_type)
                // {
                //     case 2: fd = open(name_of_file, O_APPEND | O_CREAT | O_WRONLY, 0644);
                //     dup2(fd,STDOUT_FILENO);
                //     case 3: fd = open(name_of_file, O_APPEND | O_CREAT | O_WRONLY, 0644);
                //     dup2(fd,STDOUT_FILENO);
                // }
            }
            close(piping[0]);
            if (strcmp("pinfo", brr[0]) == 0)
            {
                pinfo(brr, input);
            }
            else
            {
                write_format(brr);
            }
            exit(0);
        }
        else
        {
            wait(NULL);
             // wait(NULL) will block the parent process until any of its children has finished.
            fd = dup2(val,STDOUT_FILENO);
            close(piping[1]);
            fd = piping[0];
        }
    }
}