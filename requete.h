#ifndef __REQUETE_H__
#define __REQUETE_H__

#include "file.h"

typedef struct champ {
	int table;
	int row;
} champ;

typedef struct requete {
	file champsSortie;
	file nomsTables;
	champ * champ1;
	champ * champ2;
} requete;

extern void copierChamp(const void * valeur, void ** lieu);

extern void copierCharEtoile(const void * valeur, void ** lieu);

extern void liberer(void ** lieu);

extern requete * analyzeArgs(int argc, const char * argv[]);

extern int destroyRequete(requete **);

#endif
