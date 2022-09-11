#include "cshell.h"

// similar to that of what we did for redirecting, we do same for piping also. But we check for '|'.
int to_check_piping(char arr[])
{
    int i = 0;
    while (i < strlen(arr))
    {
        if (arr[i] == '|') // to check '|'
        {
            return 1;
        }
        i++;
    }
    return 0;
}



