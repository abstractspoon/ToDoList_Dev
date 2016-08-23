// wear.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define PATH_MAX 300

bool g_fJustOne;
const char *g_EnvVar = "PATH";

int Tokens(char *str, const char *term, char **array)
{
    char *p = strtok(str, term);
    int index = 0;
    while (p)
    {
        array[index] = p;
        ++index;
        p = strtok(0, term);
    }
    array[index] = 0;
    return index;
}
char *GetCopyEnv(const char *name)
{
    char *p = getenv(name);
    if (!p)
        return 0;
    int l = strlen(p);
    char *buffer = (char *)malloc(l + 1);
    if (buffer)
        strcpy(buffer, p);

    return buffer;
}

int ScanDir(const char *dir, const char *file)
{
    int match = 0;
    WIN32_FIND_DATA finddata;
    HANDLE find = FindFirstFile(file, &finddata);
    if (find != INVALID_HANDLE_VALUE)
    {
        do
        {
            printf("%s\\%s\n", dir, finddata.cFileName);
            match++;
            if (g_fJustOne)
                break;
        }
        while(FindNextFile(find, &finddata));
        FindClose(find);
    }
    return match;
}

bool WearFile(const char *dir, const char *file)
{
    bool fFound = false;
    const char *dot = strrchr(file, '.');
    const char *slash = strrchr(file, '/');
    const char *backslash = strrchr(file, '\\');
    
    if (dot)
    {   // If the dot is part of the path and not the filename, then ignore it.
        if (slash && dot < slash)
            dot = 0;
            
        if (backslash && dot < backslash)
            dot = 0;
    }
    if (!SetCurrentDirectory(dir))
        return false;

    if (dot)
    {
        // The user has specified an extension, don't apply the list of executable extensions.
        fFound = ScanDir(dir, file) > 0;
    }
    else
    {
        char *pathext = GetCopyEnv("PATHEXT");
        if (!pathext)
        {
            pathext = strdup(".COM;.EXE;.BAT;.CMD");
        }
        char *extarray[500];
        Tokens(pathext, ";", extarray);
        int maxextlength = 0;
        int index;
        for(index = 0; extarray[index]; ++index)
        {
            int l = strlen(extarray[index]);
            if (l > maxextlength)
                maxextlength = l;
        }


        int filenamelength = strlen(file);
        char *namebuf = (char *)malloc(filenamelength + maxextlength + 1);
        strcpy(namebuf, file);
        for(index = 0; extarray[index] && (!g_fJustOne || !fFound); ++index)
        {
            strcpy(namebuf + filenamelength, extarray[index]);
            if (ScanDir(dir, namebuf))
                fFound = true;
        }

        free(pathext);
    }
    return fFound;
}

void Usage(int argc, char* argv[])
{
    char *name = strrchr(argv[0], '\\');
    if (!name)
        name = argv[0];
        
    if (!name)
        name = "wear.exe";
        
    printf("%s - 2001 Joe Porkka\n", name);
    printf("%s file [-1] [-var envvar] [files...]\n", name);
    printf("\tSearches PATH for matching executable file (defined by PATHEXT).\n");
    printf("\tPrints all matches, in order.\n");
    printf("\tWildcards are supported.\n");
    printf("\t\n");
    printf("\t-1    Print only the first match.\n");
    printf("\t-var  Allows searching paths from a different environment variable.\n");
    printf("\t      This is useful to search for include files for example.\n");
    printf("\n");

    exit(1);
}

int main(int argc, char* argv[])
{
    int i;
    char cwd[PATH_MAX];
    GetCurrentDirectory(PATH_MAX, cwd);
    try
    {
        if (argc == 1)
            Usage(argc, argv);

        for(i = 1; i < argc; ++i)
        {
            if (*argv[i] == '-' || *argv[i] == '/')
            {
                if (strcmp(argv[i] + 1 , "?") == 0)
                    Usage(argc, argv);

                if (strcmp(argv[i] + 1, "h") == 0)
                    Usage(argc, argv);

                if (strcmp(argv[i] + 1, "1") == 0 || strcmp(argv[i] + 1 , "one") == 0)
                {
                    g_fJustOne = true;
                    argv[i] = 0;
                }

                if (i + 1 < argc && strcmp(argv[i] + 1, "var") == 0)
                {
                    argv[i] = 0;
                    ++i;
                    g_EnvVar = argv[i];
                    argv[i] = 0;
                }
            }
            if (argv[i])
            {
                if (strchr(argv[i], '/') || strchr(argv[i], '\\') || strchr(argv[i], ':'))
                    Usage(argc, argv);
            }
        }
        char *path = GetCopyEnv(g_EnvVar);
        if (path)
        {
            char *directories[512];
            Tokens(path, ";", directories);
            for(i = 1; i < argc; ++i)
            {
                if (!argv[i])
                    continue;
                    
                SetCurrentDirectory(cwd);
                bool fFound = WearFile(".", argv[i]);

                for(int index = 0; directories[index] && (!g_fJustOne || !fFound); ++index)
                {
                    int l = strlen(directories[index]);
                    if (l > 2 && (directories[index][l-1] == '\\' || directories[index][l-1] == '/'))
                        directories[index][l-1] = 0;

                    SetCurrentDirectory(cwd);
                    if (WearFile(directories[index], argv[i]))
                        fFound = true;
                }

                if (fFound)
                    printf("\n");
            }
            free(path);
        }
        else
        {
            for(i = 1; i < argc; ++i)
            {
                if (!argv[i])
                    continue;
                    
                if (WearFile(".", argv[i]))
                    printf("\n");
            }
        }
	    return 0;
    }
    catch(long err)
    {
        printf("Error %d\n", err);
        return 1;
    }
    SetCurrentDirectory(cwd);
    return 0;
}

