//
// Created by yair on 4/19/23.
//
#include "codec.h"
#include <ctype.h>
#include <string.h>
#define ASCII_SIZE 127
char * encode(char * str, int size)
{
    for(int i=0; i< size; i++)
    {
        if(str[i]+3 <= ASCII_SIZE)
        {
            str[i] += 3;
        }
        else
        {
            str[i] = (str[i]+3) % ASCII_SIZE;
        }
    }
    return str;
}

char * decode(char * str, int size)
{
    for(int i=0; i < size; i++)
    {
        str[i] = (str[i] - 3) % ASCII_SIZE;
    }
    return str;
}
