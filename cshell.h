#include <unistd.h>
#include <grp.h>
#include <signal.h>
#include <limits.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/wait.h>
#include <err.h>
#include <errno.h>
#include <error.h>
#include <time.h>
#include <fcntl.h>
typedef struct jobs
{
    char name[1000];
    pid_t pid;
} jb;

jb temp;
struct jobs ja[10000];
int bg_pid[100];
int temp1;
char bg[100][100];

int cd(char command[1000], char home[1000], char cwd[1000]);
void pwd();
int pinfo(char **token, char *input);
void foreground(char **arr);
void echo(char **command);
int ls(char cwd[1000]);
int ls_a(char cwd[500]);
int ls_l(char cwd[500]);
int ls_al(char cwd[500]);
int pinfo(char **token, char *input);

