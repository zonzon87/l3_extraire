#include <stdlib.h>

#include "earray.h"


/* Vérifié. */
void creerXEArray(xEArray ** xEA, int nbElements, void (* copier)(const void * valeur, void ** lieu), void (* liberer)(void ** lieu)) {
	int i;

	(* xEA) = (xEArray *) malloc(sizeof (xEArray));
	(* xEA)->nbChs = nbElements;
	(* xEA)->chs = (void **) malloc((sizeof (void *)) * nbElements);
	for (i = 0; i < nbElements; i++) {
		(* xEA)->chs[i] = NULL;
	}
	(* xEA)->copier = copier;
	(* xEA)->liberer = liberer;
}

/* Vérifié. */
void * accesXEArray(xEArray * xEA, int index) {
	return xEA->chs[index];
}

/* Vérifié. */
int ajouterXEArray(xEArray * xEA, int index, const void * valeur) {
	if (xEA == NULL) {
		return EARRAY_NOT_A_EARRAY;
	}

	/* Si l'index est erroné. */
	if ((index < 0) || (index > (xEA->nbChs - 1))) {
		return EARRAY_WRONGINDEX;
	}

	/* Si une valeur est déjà présente. */
	if (xEA->chs[index] != NULL) {
		/* On libère l'ancienne valeur. */
		(* (xEA->liberer))((void **) &(xEA->chs[index]));
	}
	/* On copie la nouvelle valeur. */
	(* (xEA->copier))(valeur, (void **) &(xEA->chs[index]));

	return 0;
}

/* Vérifié. */
void copierXEArray(const void * valeur, void ** lieu) {
	int i;
	xEArray * xEAIn = NULL;
	xEArray * xEAOut = NULL;

	if (valeur != NULL) {
		xEAIn = (xEArray *) valeur;
		creerXEArray(&xEAOut, xEAIn->nbChs, xEAIn->copier, xEAIn->liberer);
		for (i = 0; i < xEAOut->nbChs; i++) {
			(* (xEAIn->copier))((void *) xEAIn->chs[i], (void **) &(xEAOut->chs[i]));
		}
	}
	(* lieu) = xEAOut;
}

/* Vérifié. */
void libererXEArray(void ** lieu) {
	if ((* lieu) != NULL) {
		{
			int i;
			xEArray * xEA = NULL;

			xEA = (xEArray *) (* lieu);
			for (i = 0; i < xEA->nbChs; i++) {
				(* (xEA->liberer))((void **) &(xEA->chs[i]));
			}
			free(xEA->chs);
			xEA->chs = NULL;
			free(xEA);
			xEA = NULL;
		}
	}
}
