//
// Created by yair on 4/19/23.
//

#include "codec.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
char * encode(char * str, int size)
{
    for(int i=0; i < size; i++)
    {
        if(isupper(str[i]))
        {
            str[i] = tolower(str[i]);
        }
        else if(islower(str[i]))
        {
            str[i] = toupper(str[i]);
        }
    }
    return str;
}

char * decode(char * str, int size)
{
    return encode(str, size);
}