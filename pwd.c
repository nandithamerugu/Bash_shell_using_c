#include "cshell.h"

void pwd()
{
    char present_working_directory[1000];
    getcwd(present_working_directory, sizeof(present_working_directory));
    printf("%s\n", present_working_directory);
}