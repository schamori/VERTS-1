#include <limits.h>
#include <iostream>
#include <string.h>
#include <list>
#include <unistd.h>
#include <list>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>

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

void printFound(std::string fileName, std::string directoryName)
{
    std::cout << (int)getpid() << ": " << fileName << ": " << directoryName << (directoryName.back() == '/' ? "" : "/") << fileName << std::endl;
}

char *joinPaths(const char *path1, const char *path2) {
    // Calculate the total length needed for the new path
    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);

    char *result = (char *)malloc(len1 + 1 + len2 + 1);

    if (result == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    strcpy(result, path1);

    if (path1[len1 - 1] != '/') {
        strcat(result, "/");
    }

    // Remove any leading '/' character from the second path
    while (*path2 == '/') {
        path2++;
    }

    // Concatenate the second path
    strcat(result, path2);

    return result;
}


void search(char *directoryName, std::string fileName, bool caseInSensitive)
{
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
        
        char *currentName = direntp->d_name;
        char *joinedPath = joinPaths(directoryName, currentName);
        if (isPathDirectory(joinedPath)){
            free(joinedPath);

            continue;
        }
        free(joinedPath);


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
        std::string filename(argv[optind]);
        filenames.push_back(filename);
        optind++;
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