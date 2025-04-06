#ifndef INBUILT_H_   
#define INBUILT_H_

#include <alias.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// Used for inbuilt alias command
int runAlais(HashItem *table[], char *input1, const char *input2);
// Used for inbuilt cd command
int changeDir(const char *input1);
// Used for path command
int pathFunc(int option, const char* input);


char** str_split(char* a_str, int *length, const char a_delim);

#endif 
