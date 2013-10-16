#ifndef __REQUETE_H__
#define __REQUETE_H__

#include "file.h"

typedef struct champ {
	char c;
	int n;
} champ;

typedef struct parameters {
	file champsSortie;
	char * fichier1;
	char * fichier2;
	champ * champ1;
	champ * champ2;
} parameters;

extern void copierChamp (const void * valeur, void ** lieu);

extern void libererChamp (void ** lieu);

extern parameters * analyzeArgs(int argc, char * argv[]);

#endif
