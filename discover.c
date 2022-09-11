#include "cshell.h"


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
                discover_d_f(path, size);
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


void discover_f(char *path, size_t size)
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
        if (isFile(entry->d_name) == 1)
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
                discover_f(path, size);
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