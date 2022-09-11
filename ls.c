#include "cshell.h"

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