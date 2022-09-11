#include "cshell.h"

int cd(char command[1000], char home[1000], char cwd[1000])
{
    int result;
    if (command == NULL || strcmp(command, "~") == 0) // if only cd is entered the it has to go one step back
    {
        result = chdir(home);
    }
    else
    {
        result = chdir(command);
    }
    if (result != 0)
    {
        perror("Error");
        return -1;
    }
    return 1;
}