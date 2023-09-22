
#include <iostream>
#include <string.h>
#include <list>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
int main(int argc, char *argv[])
{
    pid_t pid, wpid;
    bool recursive = false;
    bool sensetive = false;
    string searchpath = "";
    list<string> filenames;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-R") == 0)
        {
            recursive = true;
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            sensetive = true;
        }
        else if (searchpath != "")
        {
            searchpath = argv[i];
        }
        else
        {
            filenames.push_back(argv[i]);
        }
    }
    if (filenames.empty())
    {
        cout << "Usage: ./myfind [-R] [-i] searchpath filename1 [filename2] …[filenameN]" << endl;
        return 0;
    }
    if (searchpath == "")
    {
        cout << "Usage: ./myfind [-R] [-i] searchpath filename1 [filename2] …[filenameN]" << endl;
        return 0;
    }
    for (const auto filename : filenames)
    {
        pid = fork();
        switch (pid)
        {
        case 1: /* error */
            fprintf(stderr, "myfork: error when forking child process\n");
            return EXIT_FAILURE;
        case 0: /* child process */
            printf("--- Child process with PID %d\n", (int)getpid());
            return 3;
        default: /* parent */
            printf("Parent process with PID %d\n", (int)getpid());
        }
    }