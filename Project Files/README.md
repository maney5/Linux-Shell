# Major 2

## Group Members:
1. Gustavo Platas  
2. John Gitahi  
3. Manuel Flores  

## Design Overview
- This program is a complete linux shell that has many of the functionalities
expected with bash. It's coded entirely in C.
Main check to see if arguments are present
if there arguments it'll enter batch mode if not it'll enter interactive mode. In interactive mode
commands entered into the terminal will be captured by the takeinput fuction and parsed by the parsed function
the parsed string which is a pointer to multiple characters arrays is then sent to the execute funtion where most
of the work is done dependin gon what the command is.


Compile it using the make file provided.
use ./newshell to run interactive mode
./newshell batchfile.sh runs a bash script I have provided

## Organization of the Project
### Responsibilities


- Gustavo Platas
-Built in cd command and extend the shell with pipelining
    -  
- John Gitahi 
-I/O redirection, exit command, allais , makefile 
    -
- Manuel Flores
-Path and signal handling
    - 
    
## Known Bugs or Problems
-I/O redirection doesn't work with spaceless syntax 'ls>h.txt'
-process groups not being handling correctly
-aliases dont work if ';' is present in the quotes
-cd command fails when '_' is present in directory or file
- possible memory leaks that were causing segmentation fault that were resolved but not comprehesively tested if they may occur in other sections
