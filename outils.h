#ifndef __OUTILS_H__
#define __OUTILS_H__

#include <setjmp.h>


#define TRY do { jmp_buf ex_buf; switch(setjmp(ex_buf)){ case 0: while(1) {
#define CATCH(x) break; case x:
#define FINALLY break; } default:
#define ETRY } } while(0)
#define THROW(x) longjmp(ex_buf, x)


extern void libererSimple(void ** lieu);

extern void copierCharEtoile(const void * valeur, void ** lieu);

extern int isInVAList(char c, int argc, ...);

extern int isNumeric(const char * str, const int strLength);


#endif
