#include "externfunctions.h"

#include "stdio.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C"
{
#endif

int egeszKiir(int val)
{
    printf("%d", val);
    return 1;
}

#ifdef __cplusplus
}
#endif

int logikaiKiir(int val)
{
    if (val)
    {
        printf("igaz");
    }
    else
    {
        printf("hamis");
    }

    return 1;
}

int racionalisKiir(double val)
{
    printf("%f", val);
    return 1;
}

void kiir(char *str) { printf("%s", str); }

void ujsor() { printf("\n"); }

void sorKiir(char *str)
{
    kiir(str);
    ujsor();
}

void runtimeError(char *str)
{
    sorKiir(str);
    exit(0);
}

int abszolut(int val)
{
    return abs(val);
}