#include "cshell.h"

int to_check_redirection(char arr[])
{
    // we need to check weather is their any character is '>' or '<'. If we see them then it should return 1 if no 0.
    int i = 0;
    while (i < strlen(arr))
    {
        if (arr[i] == '>')
        {
            return 1;
        }
        if (arr[i] == '<')
        {
            return 1;
        }
        i++;
    }
    return 0;
}

int to_check_type(char arr[])
{
    int val_type = 0;
    int i = 0;
    while (i < strlen(arr))
    {
        if (arr[i] == '<') // for input case
        {
            val_type = 1;
        }
        else if (arr[i] == '>' && arr[i + 1] == '>') // for appending
        {
            val_type = 2;
        }
        else if (arr[i] == '>' && (arr[i] != '>' && arr[i + 1] != '>')) // overwrite and no appending
        {
            val_type = 3;
        }
        else if (arr[i] == '>' && arr[i] != '<') // overwrite with input way
        {
            val_type = 4;
        }
        else if (arr[i] == '>' && arr[i + 1] == '>' && arr[i] != '<') // input format along with appending
        {
            val_type = 5;
            break;
        }
        i++;
    }
    return val_type;
}

void write_format(char *arr[])
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Error");
    }
    else if (pid == 0)
    {
        int val = execvp(arr[0], arr);
        if (val == -1) // Zero is returned to the child process. (If it fails, the child process is not created.)
        {
            perror("Child process is not created!");
        }
        exit(0);
    }
    else
    {
        // wait(NULL) will block the parent process until any of its children has finished.
        wait(NULL);
    }
}

void redirection(char arr[], char *input)
{
    // The below value will return the type of text written.
    int val_type = to_check_type(arr);

    char *temp[100];
    char *inp;
    // The below will seperate the input sentence using < , >> and >
    inp = strtok(arr, " < >>  > \n");

    int i = 0;
    while (inp != NULL)
    {
        temp[i] = inp;
        inp = strtok(NULL, " < >>  > \n");
        i++;
    }
    int last_index = i;
    int last = last_index - 1;
    temp[i] = inp; // Now temp will store all the words ex: echo Hello > output.txt. So temp will store echo, Hello and output.txt.
    int fd;
    char name[200];
    int val;

    // Now we need to focus on each parameter that was based on the value type
    if (val_type == 1)
    {
        write_format(temp);
    }
    else if (val_type == 2)
    {
        strcpy(name, temp[last]);
        fd = open(name, O_APPEND | O_CREAT | O_WRONLY, 0644);
        // Initially the output is pointed to stdout but we need to point that to the file we opened that is above
        dup2(STDOUT_FILENO, val);
        dup2(fd, STDOUT_FILENO);
        temp[last] = NULL;
        if (strcmp("pinfo", temp[0]) == 0)
        {
            pinfo(temp, input);
        }
        else
        {
            write_format(temp);
        }
        dup2(val, STDOUT_FILENO);
    }
    else if (val_type == 3)
    {
        strcpy(name, temp[last]);
        fd = open(name, O_TRUNC | O_CREAT | O_WRONLY, 0644);
        // Initially the output is pointed to stdout but we need to point that to the file we opened that is above
        dup2(STDOUT_FILENO, val);
        dup2(fd, STDOUT_FILENO);
        temp[last] = NULL;
        if (strcmp("pinfo", temp[0]) == 0)
        {
            pinfo(temp, input);
        }
        else
        {
            write_format(temp);
        }
        dup2(val, STDOUT_FILENO);
    }
    else
    {
        if (val_type == 4)
        {
            strcpy(name, temp[last]);
            fd = open(name, O_TRUNC | O_CREAT | O_WRONLY, 0644);
            // Initially the output is pointed to stdout but we need to point that to the file we opened that is above
            dup2(STDOUT_FILENO, val);
            dup2(fd, STDOUT_FILENO);
            temp[last] = NULL;
            if (strcmp("pinfo", temp[0]) == 0)
            {
                pinfo(temp, input);
            }
            else
            {
                write_format(temp);
            }
            dup2(val, STDOUT_FILENO);
        }
        else
        {
            strcpy(name, temp[last]);
            fd = open(name, O_APPEND | O_CREAT | O_WRONLY, 0644);
            // Initially the output is pointed to stdout but we need to point that to the file we opened that is above
            dup2(STDOUT_FILENO, val);
            dup2(fd, STDOUT_FILENO);
            temp[last] = NULL;
            if (strcmp("pinfo", temp[0]) == 0)
            {
                pinfo(temp, input);
            }
            else
            {
                write_format(temp);
            }
            dup2(val, STDOUT_FILENO);
        }
    }
}
