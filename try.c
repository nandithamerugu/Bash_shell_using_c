#include "cshell.h"
#define BLUE(string) "\x1b[34m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"
int p = 1;

int num_of_lines()
{
    FILE *fp;
    char ch;
    int linesCount = 0;
    fp = fopen("text.txt", "r");
    if (fp == NULL)
    {
        return -1;
    }
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
            linesCount++;
    }
    fclose(fp);
    return linesCount;
}

int compare(char a[], char b[])
{
    int flag = 0, i = 0;                 // integer variables declaration
    while (a[i] != '\0' && b[i] != '\0') // while loop
    {
        if (a[i] != b[i])
        {
            flag = 1;
            break;
        }
        i++;
    }
    if (flag == 0)
        return 0;
    else
        return 1;
}

int isDir(const char *fileName)
{
    struct stat path;

    stat(fileName, &path);

    return S_ISREG(path.st_mode);
}

int isFile(const char *name)
{
    DIR *directory = opendir(name);

    if (directory != NULL)
    {
        closedir(directory);
        return 0;
    }

    if (errno == ENOTDIR)
    {
        return 1;
    }

    return -1;
}

int isExecutable(const char *name)
{
    struct stat properties;
    if (stat(name, &properties) == 0 && properties.st_mode & S_IXUSR)
    {
        return 1;
    }
    return 0;
}

int cd(char command[1000], char home[1000], char cwd[1000])
{
    char hdir[1024];
    char buff[1000];
    // strcpy(lastdir,cwd);
    getcwd(buff, sizeof(buff));
    strcpy(hdir, cwd);
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
// void echo(char **commads,int start)
// {
//     for (int i = start; i < sizeof(commads) && commads[i]!=NULL; i++)
//     {
//         printf("%s ",commads[i]);
//     }
//     printf("\n");
// }

void pwd()
{
    char present_working_directory[1000];
    getcwd(present_working_directory, sizeof(present_working_directory));
    printf("%s\n", present_working_directory);
}

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

int ls(char cwd[1000])
{
    // A dirent structure contains the character pointer d_name, which points to a string that gives the name of a file in the directory.
    struct dirent *files;
    DIR *directory = opendir(cwd);
    if (directory == NULL)
    {
        return 0;
    }
    while ((files = readdir(directory)) != NULL)
    {
        // hidden files are named with '.'
        if (files->d_name[0] != '.')
        {
            int ret = isDir(files->d_name);
            if (ret == 0)
            {

                printf(BLUE("%s\n"), files->d_name); // Blue for directories
            }
            else if (isExecutable(files->d_name) == 1)
            {
                printf(RED("%s\n"), files->d_name); // executable in red
            }
            else
            {
                printf("%s\n", files->d_name);
            }
        }
    }
    closedir(directory);
    return 0;
}

// The below should print all the files including the hidden files
int ls_a(char cwd[500])
{
    // A dirent structure contains the character pointer d_name, which points to a string that gives the name of a file in the directory.
    struct dirent *files;
    DIR *directory = opendir(cwd);
    if (directory == NULL)
    {
        return 0;
    }
    while ((files = readdir(directory)) != NULL)
    {
        int ret = isDir(files->d_name);
        if (ret == 0)
        {

            printf(BLUE("%s\n"), files->d_name); // Blue for directories
        }
        else if (isExecutable(files->d_name) == 1)
        {
            printf(RED("%s\n"), files->d_name); // executable in red
        }
        else
        {
            printf("%s\n", files->d_name);
        }
    }
    closedir(directory);
    return 0;
}

int ls_l(char cwd[500])
{
    struct dirent *files;
    char name[500];
    struct stat properties;
    DIR *directory = opendir(cwd);
    long long int val = 0;
    if (directory == NULL)
    {
        return 0;
    }
    while ((files = readdir(directory)) != NULL)
    {
        // hidden files are named with '.'
        if (files->d_name[0] != '.')
        {
            strcpy(name, files->d_name);
            stat(name, &properties);
            val = val + (properties.st_blocks);
        }
    }
    closedir(directory);
    long long int res = val / 2;
    printf("total %lld\n", res);
    directory = opendir(cwd);
    if (directory == NULL)
    {
        return 0;
    }

    while ((files = readdir(directory)) != NULL)
    {
        if (files->d_name[0] != '.')
        {
            char temp[1000];
            strcpy(name, files->d_name);
            stat(name, &properties);
            if (S_ISDIR(properties.st_mode))
            {
                printf("d");
            }
            else
            {
                printf("-");
            }
            if (properties.st_mode & S_IRUSR)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (properties.st_mode & S_IWUSR)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (properties.st_mode & S_IXUSR)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (properties.st_mode & S_IRGRP)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (properties.st_mode & S_IWGRP)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (properties.st_mode & S_IXGRP)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (properties.st_mode & S_IROTH)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (properties.st_mode & S_IWOTH)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (properties.st_mode & S_IXOTH)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            printf(" ");
            printf("%ld ", properties.st_nlink);
            struct passwd *pw = getpwuid(properties.st_uid);
            struct group *gr = getgrgid(properties.st_gid);
            printf("%s ", gr->gr_name);
            printf("%s ", pw->pw_name);
            printf("%ld ", properties.st_size);
            strftime(temp, 100, "%b %d %H:%M", localtime(&properties.st_mtime));
            printf("%s ", temp);
            // printf("%s\n", files->d_name);
            int ret = isDir(files->d_name);
            if (ret == 0)
            {

                printf(BLUE("%s\n"), files->d_name); // Blue for directories
            }
            else if (isExecutable(files->d_name) == 1)
            {
                printf(RED("%s\n"), files->d_name); // executable in red
            }
            else
            {
                printf("%s\n", files->d_name);
            }
        }
    }
    closedir(directory);
    return 0;
}

int ls_al(char cwd[500])
{
    struct dirent *files;
    char name[500];
    struct stat properties;
    DIR *directory = opendir(cwd);
    long long int val = 0;
    if (directory == NULL)
    {
        return 0;
    }
    while ((files = readdir(directory)) != NULL)
    {

        strcpy(name, files->d_name);
        stat(name, &properties);
        val = val + (properties.st_blocks);
    }
    closedir(directory);
    long long int res = val / 2;
    printf("total %lld\n", res);
    directory = opendir(cwd);
    if (directory == NULL)
    {
        return 0;
    }

    while ((files = readdir(directory)) != NULL)
    {

        char temp[1000];
        strcpy(name, files->d_name);
        stat(name, &properties);
        if (S_ISDIR(properties.st_mode))
        {
            printf("d");
        }
        else
        {
            printf("-");
        }
        if (properties.st_mode & S_IRUSR)
        {
            printf("r");
        }
        else
        {
            printf("-");
        }
        if (properties.st_mode & S_IWUSR)
        {
            printf("w");
        }
        else
        {
            printf("-");
        }
        if (properties.st_mode & S_IXUSR)
        {
            printf("x");
        }
        else
        {
            printf("-");
        }
        if (properties.st_mode & S_IRGRP)
        {
            printf("r");
        }
        else
        {
            printf("-");
        }
        if (properties.st_mode & S_IWGRP)
        {
            printf("w");
        }
        else
        {
            printf("-");
        }
        if (properties.st_mode & S_IXGRP)
        {
            printf("x");
        }
        else
        {
            printf("-");
        }
        if (properties.st_mode & S_IROTH)
        {
            printf("r");
        }
        else
        {
            printf("-");
        }
        if (properties.st_mode & S_IWOTH)
        {
            printf("w");
        }
        else
        {
            printf("-");
        }
        if (properties.st_mode & S_IXOTH)
        {
            printf("x");
        }
        else
        {
            printf("-");
        }
        printf(" ");
        printf("%ld ", properties.st_nlink);
        struct passwd *pw = getpwuid(properties.st_uid);
        struct group *gr = getgrgid(properties.st_gid);
        printf("%s ", gr->gr_name);
        printf("%s ", pw->pw_name);
        printf("%ld ", properties.st_size);
        strftime(temp, 100, "%b %d %H:%M", localtime(&properties.st_mtime));
        printf("%s ", temp);
        // printf("%s\n", files->d_name);
        int ret = isDir(files->d_name);
        if (ret == 0)
        {

            printf(BLUE("%s\n"), files->d_name); // Blue for directories
        }
        else if (isExecutable(files->d_name) == 1)
        {
            printf(RED("%s\n"), files->d_name); // executable in red
        }
        else
        {
            printf("%s\n", files->d_name);
        }
    }
    closedir(directory);
    return 0;
}

void run()
{
    int x;
    pid_t pid = waitpid(-1, &x, WNOHANG);
    char temp[100];
    int check = 0;
    if (pid > 0)
    {
        for (int j = 0; j < p; j++)
        {
            if (bg_pid[j] == pid)
            {
                strcpy(temp, bg[j]);
                check = 1;
                break;
            }
        }
        if (WEXITSTATUS(x) == 0 && WIFEXITED(x) && check == 1)
        {
            printf("%d exited normally\n", pid);
        }
    }
    return;
}

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

void background(char **arr, int last_index, int time)
{
    pid_t pid1;
    arr[last_index - 1] = NULL;
    pid1 = fork();
    // setpgid will reassign the process to another process group
    // setpgid(0, 0); // first parameter is the process that we want to change and second parameter is the new pid
    if (pid1 < 0)
    {
        perror("Failed: Fork operation failed");
    }
    else if (pid1 == 0)
    {
        int status;
        pid_t pid2 = getpid();
        if (strcmp(arr[0], "sleep") == 0)
        {
            waitpid(pid1, &status, WUNTRACED);

            if (WIFSTOPPED(status))
            {
                printf("%d exited normally\n", pid1);
            }
        }
        sleep(time);
        printf("\nsleep with pid = %d exited normally # After %d seconds\n", pid2, time);
    }
    else
    {
        temp1++;
        printf("[%d] %d\n", temp1, pid1);
        ja[temp1].pid = pid1;
        strcpy(ja[temp1].name, arr[0]);
    }
    return;
}

void background1(int start_index, char **token, int last_index)
{
    int check = 0;
    int status;
    if (strcmp(token[last_index - 1], "&") == 0)
    {
        check = 1;
    }
    char **list;
    if (check == 1)
    {
        int i = 0;
        while (i < last_index - 1)
        {
            list[i] = token[i];
            i++;
        }
        list[i] = NULL;
    }
    else
    {
        int i = 0;
        while (i < last_index)
        {
            list[i] = token[i];
            i++;
        }
        list[i] = NULL;
    }

    int pid;
    pid = fork();
    if (pid < 0)
    {
        printf("forking failed");
    }
    else if (pid == 0)
    {
        if (check == 1)
        {
            // printf("1");
            if (execvp(token[0 + start_index], list) == -1)
            {
                printf("Execution failed '%s'\n", token[0 + start_index]);
                return;
            }
        }
        else
        {
            // printf("2");
            if (execvp(token[0 + start_index], token) == -1)
            {
                printf("Execution failed '%s'\n", token[0 + start_index]);
                return;
            }
        }
        printf("%d", getpgrp()); // get the process group ID of the calling process
    }
    else
    {

        if (check == 0)
        {
            // printf("3");
            int pid_temp;
            sleep(token[1][0] - 48);
            pid_temp = waitpid(pid, &status, WUNTRACED);

            if (WIFSTOPPED(status))
            {
                printf("%s with %d exited normally\n", token[0], pid);
            }

            printf("# sleeps for %d second\n", token[1][0] - 48);
        }
        else
        {
            // printf("4");
            bg_pid[p] = pid;
            strcpy(bg[p], token[0]);
            printf("%d\n", bg_pid[p]);
            p++;
        }
    }
}

void discover_d(char *path, size_t size)
{
    DIR *dir;
    struct dirent *entry;
    size_t len = strlen(path);

    if (!(dir = opendir(path)))
    {
        fprintf(stderr, "path not found: %s: %s\n",
                path, strerror(errno));
        return;
    }

    puts(path);
    while ((entry = readdir(dir)) != NULL)
    {
        char *name = entry->d_name;
        if (isDir(entry->d_name) == 0)
        {
            if (!strcmp(name, ".") || !strcmp(name, ".."))
                continue;
            if (len + strlen(name) + 2 > size)
            {
                // fprintf(stderr, "path too long: %s/%s\n", path, name);
                printf("%s/%s\n", path, name);
            }
            else
            {
                path[len] = '/';
                strcpy(path + len + 1, name);
                discover_d(path, size);
                path[len] = '\0';
            }
        }
        // else
        // {
        //     printf("%s/%s\n", path, name);
        // }
    }
    closedir(dir);
}

void discover(char *path, size_t size)
{
    DIR *dir;
    struct dirent *entry;
    size_t len = strlen(path);

    if (!(dir = opendir(path)))
    {
        fprintf(stderr, "path not found: %s: %s\n",
                path, strerror(errno));
        return;
    }

    puts(path);
    while ((entry = readdir(dir)) != NULL)
    {
        char *name = entry->d_name;
        if (isDir(entry->d_name) == 0)
        {
            if (!strcmp(name, ".") || !strcmp(name, ".."))
                continue;
            if (len + strlen(name) + 2 > size)
            {
                // fprintf(stderr, "path too long: %s/%s\n", path, name);
                // printf("%s/%s\n", path, name);
            }
            else
            {
                path[len] = '/';
                strcpy(path + len + 1, name);
                discover(path, size);
                path[len] = '\0';
            }
        }
        else
        {
            printf("%s/%s\n", path, name);
        }
    }
    closedir(dir);
}

void discover_d_f(char *path, size_t size)
{
    DIR *dir;
    struct dirent *entry;
    size_t len = strlen(path);

    if (!(dir = opendir(path)))
    {
        fprintf(stderr, "path not found: %s: %s\n",
                path, strerror(errno));
        return;
    }

    puts(path);
    while ((entry = readdir(dir)) != NULL)
    {
        char *name = entry->d_name;
        if (isDir(entry->d_name) == 0 || isFile(entry->d_name) == 1)
        {
            if (!strcmp(name, ".") || !strcmp(name, ".."))
                continue;
            if (len + strlen(name) + 2 > size)
            {
                // fprintf(stderr, "path too long: %s/%s\n", path, name);
                printf("%s/%s\n", path, name);
            }
            else
            {
                path[len] = '/';
                strcpy(path + len + 1, name);
                discover_d(path, size);
                path[len] = '\0';
            }
        }
        // else
        // {
        //     printf("%s/%s\n", path, name);
        // }
    }
    closedir(dir);
}

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


int main(int argc, char *argv[])
{

    // printf("%s",argv[0]);
    char computer[1000];  // will store the present directory
    char user_name[1000]; // will store the user_name/computer_name
    gethostname(computer, sizeof(computer));
    // printf("%s\n",computer);
    getlogin_r(user_name, sizeof(user_name));
    // printf("%s\n",user_name);
    char cwd[1000]; // will store current working directory
    char home[1000];
    char lastdir[1000]; // will keep track on the previous directory used in cd ..
    getcwd(home, sizeof(home));
    getcwd(lastdir, sizeof(lastdir));
    // printf("%s\n",cwd);
    // printf("%s\n",lastdir);
    char *input[1000];
    int v = 0;
    int count = 0;
    while (1)
    {
        run();
        // printf("%s\n","here");
        getcwd(cwd, sizeof(cwd));
        // printf("%s\n",cwd);
        if (strlen(home) == strlen(cwd))
        {
            printf("<%s@%s:~> ", user_name, computer);
        }
        else if (strlen(home) > strlen(cwd))
        {
            printf("<%s@%s:~%s> ", user_name, computer, cwd);
        }
        else if (strlen(home) < strlen(cwd))
        {
            printf("<%s@%s:~", user_name, computer);
            int i = strlen(home);
            while (cwd[i] != '\0')
            {
                printf("%c", cwd[i]);
                i++;
            }
            printf("> ");
        }
        // We need to write as if we write in normal terminal
        char content[1000];
        char hist[1000];
        char *pointer = fgets(content, sizeof(content), stdin);
        //printf("%s", pointer);

        int len = strlen(content);

        if (len == 1)
        {
            // printf("\n");
            continue;
        }
        // ***************History*************
        if (strcmp(cwd, home) == 0)
        {
            int count = 0;
            int value = num_of_lines();
            count = value;
            FILE *file;
            char buffer[1000];
            file = fopen("text.txt", "r");
            fgets(buffer, sizeof(buffer), file);
            fclose(file);
            char arr[1000];
            file = fopen("text.txt", "r");
            for (int i = 1; i <= count; i++)
            {
                fgets(buffer, sizeof(buffer), file);
            }
            // printf("%s", buffer);
            fclose(file);
            if (count <= 20)
            {
                // printf("%d", compare(arr,buffer));
                if (compare(content, buffer) != 0)
                {
                    // printf("1");
                    file = fopen("text.txt", "a");
                    fprintf(file, "%s", content);
                    fclose(file);
                    // printf("1");
                    // val = num_of_lines();
                    count++;
                }
                else
                {
                    // printf("2");
                    count++;
                }
            }
            else
            {
                // printf("2");
                count = 0;
                file = fopen("text.txt", "w");
                fprintf(file, "%s", content);
                fclose(file);
            }
        }

        int len_of_content = strlen(content);
        // printf("%s\n",content);
        // printf("%d\n",len_of_content);
        char *temp[1000]; // This is to store the all the words that are written in the shell
        const char s[2] = ";";
        char *token;
        /* get the first token */
        token = strtok(content, ";");
        /* walk through other tokens */
        int val = 0;
        while (token != NULL)
        {
            temp[val] = token;

            token = strtok(NULL, ";");
            val++;
        }
        temp[val] = token; // say if cd Desktop temp[0] = cd and temp[1] = Desktop
        // printf("%s",temp[1]);
        //  we copied the strings from temp to input. Highly used while implementing ls command
        int l_val = 0;
        int a_val = 0;
        int al_val = 0;
        int d_val = 0;
        int d_empty = 0;
        int f_val = 0;
        char arr1[1000];
        char path[1000][1000];
        int size = 0;
        // printf("%s",token);
        for (int i = 0; i < val; i++)
        {
            if(to_check_redirection(temp[i]))
            {
                redirection(temp[i],input[0]);
                continue;
            }

            // printf("%s", hist[v]);
            const char s1[4] = " \n\t";
            char *token1;
            /* get the first token */
            token1 = strtok(temp[i], s1);
            int j = 0;
            while (token1 != NULL)
            {
                input[j] = token1;
                token1 = strtok(NULL, s1);
                j++;
            }
            input[j] = token1;
            int op = j;
            // printf("%d\n",op);
            //  printf("%s",input[1 printf( " %s\n", token );]);

            if (strcmp(input[0], "cd") == 0)
            {
                // printf("%s\n",cwd);
                if (input[1] != NULL && strcmp(input[1], "-") == 0)
                {
                    printf("%s\n", lastdir);
                    break;
                }
                strcpy(lastdir, cwd);
                cd(input[1], home, cwd);
                // printf("%s\n",cwd);
            }
            else if (strcmp(input[0], "echo") == 0)
            {
                echo(input);
            }
            else if (strcmp(input[0], "pwd") == 0)
            {
                pwd();
            }
            else if (strcmp(input[0], "history") == 0)
            {
                FILE *f;
                char s;
                f = fopen("text.txt", "r");
                while ((s = fgetc(f)) != EOF)
                {
                    printf("%c", s);
                }
                fclose(f);
            }
            else if (strcmp(input[0], "exit") == 0)
            {
                exit(0);
            }
            else if (strcmp(input[0], "pinfo") == 0)
            {
                pinfo(input, argv[0]);
            }
            else if (strcmp(input[0], "ls") == 0)
            {
                for (int k = 1; k < j; k++)
                {
                    if (strcmp(input[k], "-la") == 0 || strcmp(input[k], "-al") == 0)
                    {
                        al_val = 1;
                    }
                    else if (strcmp(input[k], "-l") == 0)
                    {
                        l_val = 1;
                    }
                    else if (strcmp(input[k], "-a") == 0)
                    {
                        a_val = 1;
                    }
                    else
                    {
                        strcpy(arr1, input[k]);
                        strcpy(path[size], arr1);
                        size++;
                    }
                }
                if (size == 0)
                {
                    if (al_val == 1 || (a_val == 1 && l_val == 1))
                    {
                        // printf("1");
                        ls_al(cwd);
                    }
                    else if (a_val == 1)
                    {
                        ls_a(cwd);
                    }
                    else if (l_val == 0)
                    {
                        // printf("2");
                        ls(cwd);
                    }
                    else if (l_val == 1)
                    {
                        ls_l(cwd);
                    }
                }
                else
                {
                    char t[1000];
                    strcpy(t, cwd);
                    for (int l = 0; l < size; l++)
                    {
                        if (l > 0)
                        {
                            printf("\n\n");
                        }
                        if (size > 1)
                        {
                            printf("%s:\n", path[l]);
                        }

                        int para = cd(path[i], home, cwd);
                        char direc[1000];
                        getcwd(direc, sizeof(direc));

                        if (para != -1)
                        {
                            if (al_val == 1 || (a_val == 1 && l_val == 1))
                            {
                                // printf("1");
                                ls_al(direc);
                                cd(t, home, cwd);
                            }
                            else if (a_val == 1)
                            {
                                ls_a(direc);
                                cd(t, home, cwd);
                            }
                            else if (l_val == 0)
                            {
                                // printf("1");
                                ls(direc);
                                cd(t, home, cwd);
                            }
                            else if (l_val == 1)
                            {
                                ls_l(direc);
                                cd(t, home, cwd);
                            }
                        }
                    }
                }
            }
            else if (strcmp(input[0], "discover") == 0)
            {
                for (int k = 1; k < j; k++)
                {
                    if (strcmp(input[k], "-d") == 0)
                    {
                        d_val = 1;
                    }
                    if (strcmp(input[k], "-f") == 0)
                    {
                        f_val = 1;
                    }
                }
                if (d_val == 1 && f_val == 1)
                {
                    char path[1024] = ".";
                    discover(path, sizeof path);
                }
                else if (d_val == 1)
                {
                    char path[1024] = ".";
                    discover_d(path, sizeof path);
                }
                else
                {
                    char path[1024] = ".";
                    discover(path, sizeof path);
                }
            }
            else
            {

                // foreground(input);
                // *********************
                // char test[10];
                // strcpy(test, input[1]);
                // int len = strlen(test);
                // char w;
                // for (int k = 0; k < len; k++)
                // {
                //     w = test[k];
                // }
                // char str[3] = "";
                // for(int k=0;k<len-1;k++)
                // {
                //     strncat(str,&test[k],1);
                // }
                // int val = atoi(str);
                // //printf("%d",val);
                // if (w == '&')
                // {
                //     background(input, op, val);
                // }
                // else
                // {
                //     foreground(input);
                // }
                // printf("%c\n", w);
                // ***********
                background1(0, input, op);
                //  background(input,op);
            }
        }
    }
}