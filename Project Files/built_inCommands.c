#include <built_inCommands.h>


// String split function
char** str_split(char* a_str, int *length, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    } 

    // Add space for trailing token.
    count += last_comma < (a_str + strlen(a_str) - 1);

   // Add space for terminating null string so caller knows where the list of returned strings ends.
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);
        int returned = 0;
        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
            returned++;
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
        *length = returned;
    }
   
    return result;
}

// Used for inbuilt alias command
int runAlais(HashItem *table[], char *input1, const char *input2)
{
    
    if (!input1)
    {
        displayHashTable(table);
    }
    else if (strcmp(input1, "-c") == 0)
    {
        freeTable(table);
       
        printf("Aliases cleared");
    }
    else if (strcmp(input1, "-r") == 0)
    {
        if (!input2)
        {
            printf("No alias selected for removal");
            return 0;
        }
        else
        {
            deleteItem(table, input2);
        }
    }
    else
    {
        insertItem(table, input1, input2);
    }
    return 1;
}

// Used for inbuilt cd command
int changeDir(const char *input1)
{   
    if (input1 == NULL)
    {
        char *home = getenv("HOME");
        
        int result = chdir(home);
        if(result < 0) {
            printf("Home is not a directory\n");
        }

        return 1; 
    }
    int result = chdir(input1);
    if(result < 0) {
        printf("Not a directory, staying put\n");
    }
    return 1;
}
// Inbuilt path function
int pathFunc(int option, const char *input){
	char *path = getenv("PATH"); // gets current path
	if(option == -1){ // checks if path was properly used
		printf("Incorrect use, proper use 'path [+/-] [pathname]'");
		return 0;
	}
	else if(option == 0){ // prints out path
        	printf("%s\n", path);
	}
	else if(option == 1){ // adds to path
		strcat(path, ":");
		strcat(path, input);
		setenv("PATH", path, 1);
	}
	else if(option == 2){ // removes from path
		char *remove = (strstr(path, input)); // char pointer where removal target is
		if(remove == NULL){ // if pathname does not exist notify user
			printf("Path %s does not exist", input);
			return 0;
		}
		else{
			int len = strlen(input)+1; // get length+1 of pathname
			remove--; // get position right before target to also remove ':'
			while(remove){
				*remove = '\0';
				strcat(path, remove+len);
				remove = strstr(path, input);
			} //removes targeted pathname from PATH environmental variable
			setenv("PATH", path, 1); // set PATH to new path
		}
	}
	return 1;
}
