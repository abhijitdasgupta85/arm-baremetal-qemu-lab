#include "mystring.h"

static void my_strcpy(char *dst, const char *src)
{
    while ((*dst++ = *src++));
}

static void my_strcat(char *dst, const char *src)
{
    while (*dst) dst++;
    while ((*dst++ = *src++));
}
