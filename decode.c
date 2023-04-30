//
// Created by yair on 4/19/23.
//
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
char* getString(int size,int argc, char *argv[])
{
    char* st = (char*) malloc(sizeof(char)*size);
    if(st == NULL)
    {
        perror("Error");
        return NULL;
    }
    //Builds the string for manipulation
    for (int i = 2; i < argc; i++)
    {
        strcat(st, argv[i]);
        if(i < argc-1)
        {
            strcat(st, " ");
        }
    }
    return st;
}
int getLen(int argc, char *argv[])
{
    int size = 0;
    for(int i = 2; i < argc; i++)
    {
        size += strlen(argv[i]) + 1;
    }
    return size;
}
int getWordLen(char* str)
{
    int size = 0;
    int i = 0;
    while(str[i] != '\0')
    {
        size++;
    }
    return size+1;
}
int getFullLen(int argc, char *argv[])
{
    int size = 0;
    for(int i = 0; i < argc; i++)
    {
        size += strlen(argv[i]) + 1;
    }
    return size;
}
int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Error: Please write in the right format<decode <codec> <message>>\n");
        return 1;
    }
    //Declaring variables that we are going to use.
    typedef char* (*sym)(char *, int);
    void *handle;
    int size = getLen(argc, argv);
    char* str = getString(size, argc, argv);

    //Allocation check.
    if(str == NULL)
    {
        perror("Error");
        return errno;
    }
    size --;
    //Loads codecA.so or codecB.so
    char* lib = argv[1];
    strcat(lib, ".so");
    // Loads the shared library and stores in handle
    handle = dlopen(lib, RTLD_LAZY);
    if(handle == NULL)
    {
        printf("dlopen failed: %s\n", dlerror());
    }
    //Delivers the function decode(char* str, int size) from handle
    sym decode = (sym)dlsym(handle,"decode");
    char* decoded_str = decode(str, size);

    printf("decoded: %s\n", decoded_str);

    //Clean
    free(str);
    dlclose(handle);
    return 0;
}
