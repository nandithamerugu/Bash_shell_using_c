#include "cshell.h"

void echo(char **command)
{
    int len = sizeof(command);
    char temp[100];
    char temp1[1000];
    strcpy(temp, command[0]);
    if (strcmp(temp, "echo") == 0)
    {
        for (int i = 1; i < len && command[i] != NULL; i++)
        {
            printf("%s ", command[i]);
            strcpy(temp1, command[i]);
        }
        int len_of_string = strlen(temp1);
        if (temp1[0] == '"' && temp1[len_of_string - 1] == '"')
        {
            for (int i = 1; i < len_of_string - 1; i++)
            {
                // printf("%c", temp1[i]);
            }
            printf("\n");
        }
        else
        {
            for (int i = 0; i < len_of_string; i++)
            {
                // printf("%c", temp1[i]);
            }
            printf("\n");
        }
    }
}