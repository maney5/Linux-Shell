# Project: Linux-Shell
> Imported from Project Folder; Spring 23

## Description
A complete Linux shell with many of the functionalities expected with bash to manage files; coded entirely in C.

Below is an example of an output using redirection and pipelining:
```
Dir: /nfs/home/STUDENTS/mf0485/3600/Major2> ls -l | grep "new" > testFile

testFile:
newshell
newshell.c
newshell.o

Dir: /nfs/home/STUDENTS/mf0485/3600/Major2> echo "Hello, world!" | tee testFile2

testFile2:
Hello, newer world!
```

