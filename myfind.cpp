
#include <iostream>
#include <string.h>
#include <list>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <list>
#include <string.h>

int main(int argc, char *argv[])
{

    // Arugment passing
    int opt;
    bool recursive = false;
    bool caseInsensitive = false;
    std::list<std::string> filenames;

    while ((opt = getopt(argc, argv, "Ri")) != -1)
    {
        switch (opt)
        {
        case 'R':
            recursive = true;
            break;
        case 'i':
            caseInsensitive = true;
            break;
        default:
            std::cout << "Usage: " << argv[0] << " [-R] [-i] searchpath filename1 [filename2] …[filenameN]" << std::endl;
            return 1;
        }
    }

    if (optind >= argc)
    {
        std::cout << "Error: searchpath and filename(s) are required." << std::endl;
        return 1;
    }
    optind++;
    std::string searchPath = argv[optind];
    if (optind >= argc)
    {
        std::cout << "Error: at least one filename is required." << std::endl;
        return 1;
    }

    std::cout << "Recursive: " << (recursive ? "true" : "false") << std::endl;
    std::cout << "Case Insensitive: " << (caseInsensitive ? "true" : "false") << std::endl;
    std::cout << "Search Path: " << searchPath << std::endl;
    while (optind < argc - 1)
    {
        optind++;
        std::string filename(argv[optind]);
        std::cout << "Filename " << filename << std::endl;
        filenames.push_back(filename);
    }
    pid_t pid;

    for (const auto &filename : filenames)
    {
        std::cout << "Filename: " << filename << std::endl;
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
            wiat
        }
    }
}