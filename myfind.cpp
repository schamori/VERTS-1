
#include <iostream>
#include <string.h>
#include <list>

using namespace std;
int main(int argc, char *argv[])
{
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
    cout << recursive << sensetive << searchpath << endl;
}