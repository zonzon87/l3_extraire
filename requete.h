#ifndef __REQUETE_H__
#define __REQUETE_H__

#include "file.h"

#define TRY do { jmp_buf ex_buf; switch(setjmp(ex_buf)){ case 0: while(1) {
#define CATCH(x) break; case x:
#define FINALLY break; } default:
#define ETRY } } while(0)
#define THROW(x) longjmp(ex_buf, x)

#define MISSING_ARGS_EXCEPTION 1
#define KEYWORD_EXCEPTION 2
#define SYNTAX_CHAMP_EXCEPTION 3
#define SYNTAX_CONDITION_EXCEPTION 4
#define CONDITION_EXCEPTION 5

typedef struct champ {
	int table;
	int row;
} champ;

typedef struct condition {
	champ * champ1;
	char * comparisonOperator;
	champ * champ2;
} condition;

typedef struct requete {
	file champsSortie;
	file nomsTables;
	file conditions;
} requete;

extern void copierChamp(const void * valeur, void ** lieu);

extern void copierCharEtoile(const void * valeur, void ** lieu);

extern void liberer(void ** lieu);

extern int base26to10(int * result, const char * str, const int strLength);

extern int isNumeric(const char * str, const int strLength);

extern int parseSyntaxChamp(const char separatorChamp, const char * c, int * fileNumber, int * rowNumber);

extern requete * analyzeArgs(int argc, const char * argv[]);

extern int destroyRequete(requete **);

#endif
