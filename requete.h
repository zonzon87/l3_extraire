#ifndef __REQUETE_H__
#define __REQUETE_H__

#include "file.h"


#define MISSING_ARGS_EXCEPTION 1
#define KEYWORD_EXCEPTION 2
#define SYNTAX_CHAMP_EXCEPTION 3
#define SYNTAX_CONDITION_EXCEPTION 4
#define INCOHERENT_REQUETE_EXCEPTION 5


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
	char option;
	champ * champOrdre;
} requete;


extern void copierChamp(const void * valeur, void ** lieu);

extern int comparerChamp(const champ * ch1, const champ * ch2);

extern void copierCondition(const void * valeur, void ** lieu);

extern void libererCondition(void ** lieu);

extern void destroyRequete(requete **);

extern int base26to10(int * result, const char * str, const int strLength);

extern int parseSyntaxChamp(champ ** ch, const char separatorChamp, const char * c);

extern int parseSyntaxCondition(condition ** co, const char separatorChamp, const char * c);

extern int analyzeArgs(requete ** req, const int argc, const char * argv[]);


#endif
