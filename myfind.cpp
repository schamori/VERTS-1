#include <limits.h>
#include <iostream>
#include <string.h>
#include <list>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <list>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <cstring>

void printFound(std::string fileName, std::string directoryName)
{
    std::cout << (int)getpid() << ": " << fileName << ": " << directoryName << (directoryName.back() == '/' ? "" : "/") << fileName << std::endl;
}

bool isPathDirectory(char const *path)
{
    DIR *dir = NULL;
    dir = opendir(path);
    if (dir == NULL)
        return false;

    else
        closedir(dir);
    return true;
}

void search(char *directoryName, std::string fileName, bool caseInSensitive)
{
    char mycwd[PATH_MAX];

    if (getcwd(mycwd, PATH_MAX) == NULL)
    {
        perror("Failed to get current working directory");
        exit(1);
    }
    char relativeDirectory[PATH_MAX]; // Make sure the result array is large enough to hold the concatenated string

    std::strcpy(relativeDirectory, mycwd); // Copy the first string into the result
    std::strcat(relativeDirectory, directoryName);

    std::list<char *> directoryNames = {
        directoryName, relativeDirectory};
    std::cout << "Path: " << relativeDirectory;

    for (const auto &directoryName : directoryNames)
    {

        std::cout << directoryName;

        DIR *dirp = opendir(directoryName);
        if (dirp == NULL)
        {
            perror("Failed to get current working directory");
            exit(1);
        }
        struct dirent *direntp = readdir(dirp);

        while (direntp != NULL)
        {

            direntp = readdir(dirp);
            // Todo
            // skip directories
            // if isPathDirectory(direntp->d_name)
            //   continue;
            char *currentName = direntp->d_name;

            if (caseInSensitive && strcasecmp(currentName, fileName.c_str()) == 0)
            {
                // Pass in the current Name because the search string could have upper cases
                printFound(currentName, directoryName);
                break;
            }
            if (strcmp(currentName, fileName.c_str()) == 0)
            {
                printFound(fileName, directoryName);
                break;
            }
        }
        while ((closedir(dirp) == -1) && (errno == EINTR))
            ;
    }
}

int main(int argc, char *argv[])
{

    // Arugment passing
    int opt;
    int status;
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
    char *searchPath = argv[optind];
    optind++;
    if (optind >= argc)
    {
        std::cout << "Error: at least one filename is required." << std::endl;
        return 1;
    }

    std::cout << "Recursive: " << (recursive ? "true" : "false") << std::endl;
    std::cout << "Case Insensitive: " << (caseInsensitive ? "true" : "false") << std::endl;
    std::cout << "Search Path: " << searchPath << std::endl;

    while (optind < argc)
    {
        std::cout << argv[optind];
        std::string filename(argv[optind]);
        filenames.push_back(filename);
        optind++;
    }
    std::cout << "Test";
    for (const auto &filename : filenames)
    {
        std::cout << filename;
    }
    pid_t pid;

    for (const auto &filename : filenames)
    {

        pid = fork();
        switch (pid)
        {
        case 1: /* error */
            fprintf(stderr, "myfork: error when forking child process\n");
            return EXIT_FAILURE;
        case 0: /* child process */
            if (recursive)
            {
            }
            else
            {
                search(searchPath, filename, caseInsensitive);
            }
            return 3;
        default: /* parent */
            wait(&status);
        }
    }
}