//Gustavo Platas
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//#include "chdir.h"

int main(int argc, char* argv[]) 
{
    if(argc == 1)
    {
        const char* home_directory = getenv("HOME");
        
        if(home_directory != NULL)
        {
            int value = atoi(home_directory);
        }
        if(home_directory == NULL)
        {
            printf("Unable to determine home directory. \n");
            return 1;
        }
        if(chdir(home_directory) != 0)
        {
            printf("Unable to change to ome directory. \n");
            return 1;
        }
    }
    else if(argc == 2)
    {
        if(chdir(argv[1]) != 0)
        {
            printf("Unable to change directory '%s'. \n", argv[1]);
            return 1;
        }
    }
    else 
    {
        printf("Invalid number of arguments provided. \n");
        return 1;
    }
    printf("Current directory: %s \n", getcwd(NULL,0));

    return 0;
}
