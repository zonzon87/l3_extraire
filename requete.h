#ifndef __REQUETE_H__
#define __REQUETE_H__

#include "file.h"

typedef struct champ {
	char c;
	int n;
} champ;

typedef struct parameters {
	file champsSortie;
	file nomFichiers;
	champ * champ1;
	champ * champ2;
} parameters;

extern void copierChamp(const void * valeur, void ** lieu);

extern void copierCharEtoile(const void * valeur, void ** lieu);

extern void liberer(void ** lieu);

extern parameters * analyzeArgs(int argc, char * argv[]);

extern int destroyRequete(parameters **);

#endif
