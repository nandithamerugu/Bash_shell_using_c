#include "cshell.h"

int pinfo(char **token, char *input)
{
    pid_t pid; // it is a datatype that specifies the process ID.
    char buffer[1000];
    char path1[1000];
    char content[1000];
    char temp1[1000];
    long long char_to_int_val;
    if (token[1] == NULL)
    {
        pid = getpid();
        strcpy(buffer, input);
        buffer[0] = '~';
    }
    else
    {

        char_to_int_val = (pid_t)atoi(token[1]); // This converts the character to inteter
        pid = char_to_int_val;
        struct stat properties;
        // strcat(path1, "/proc/");
        // strcat(path1, pid);
        // strcat(path1, "/exe"); // /proc/pid/exe
        sprintf(path1, "/proc/%d/exe", pid);
        if (stat(path1, &properties) == -1 && errno == ENOENT)
        {
            printf("Error\n");
            return 0;
        }
        else
        {
            readlink(path1, content, sizeof(content)); // place the contents of the symbolic link referred to by path in the buffer buf which has size bufsize.
            strcpy(buffer, content);
        }
    }

    char path2[1000];
    char temp[1000];
    // strcat(path2, "/proc/");
    // strcat(path2, pid);
    // strcat(path2, "/status"); // /proc/pid/status
    sprintf(path2, "/proc/%d/status", pid);
    FILE *file1 = fopen(path2, "r");
    int c = 0;
    char process_status;
    while (fgets(temp, sizeof(temp), file1) != NULL)
    {
        if (c == 2)
        {
            process_status = temp[7];
        }
        c++;
    }
    fclose(file1);

    char memory[1000];
    // strcat(memory, "/proc/");
    // strcat(memory, pid);
    // strcat(memory, "/status"); // /proc/pid/memory
    sprintf(memory, "/proc/%d/statm", pid);
    FILE *file2 = fopen(memory, "r");
    fgets(temp1, sizeof(temp1), file2);
    char *mem;
    mem = strtok(temp1, "\t");
    fclose(file2);

    printf("pid : %d\n", pid);
    printf("process status : %c\n", process_status);
    printf("memory : %s\n", mem);
    printf("executable Path: %s\n", buffer);
}