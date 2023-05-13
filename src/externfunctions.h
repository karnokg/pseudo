#ifndef H_EXTERNFUNCTIONS
#define H_EXTERNFUNCTIONS

#ifdef __cplusplus
extern "C"
{
#endif

int egeszKiir(int val);

int logikaiKiir(int val);

int racionalisKiir(double val);

void kiir(char *str);

void ujsor();

void sorKiir(char *str);

void runtimeError(char *str);

#ifdef __cplusplus
}
#endif


#endif
