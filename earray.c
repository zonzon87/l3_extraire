#include <stdlib.h>

#include "earray.h"


/* Vérifié. */
void creerXEArray(xEArray ** xEA, int nbElements, void (* copier)(const void * valeur, void ** lieu), void (* liberer)(void ** lieu)) {
	int i;

	(* xEA) = (xEArray *) malloc(sizeof (xEArray));

	(* xEA)->nbElements = nbElements;
	(* xEA)->elements = (void **) malloc((sizeof (void *)) * nbElements);
	for (i = 0; i < nbElements; i++) {
		(* xEA)->elements[i] = NULL;
	}
	(* xEA)->copier = copier;
	(* xEA)->liberer = liberer;
}

/* Vérifié. */
void * accesXEArray(xEArray * xEA, int index) {
	if ((index > -1) && (index < xEA->nbElements)) {
		return xEA->elements[index];
	} else {
		return NULL;
	}
}

/* Vérifié. */
int ajouterXEArray(xEArray * xEA, int index, const void * valeur) {
	if ((xEA == NULL) || (xEA->elements == NULL)) { /*a vérifier*/
		return EARRAY_NOT_A_EARRAY;
	}

	/* Si l'index est erroné. */
	if ((index < 0) || (index > (xEA->nbElements - 1))) {
		return EARRAY_WRONGINDEX;
	}

	/* Si une valeur est déjà présente. */
	if (xEA->elements[index] != NULL) {
		/* On libère l'ancienne valeur. */
		(* (xEA->liberer))((void **) &(xEA->elements[index]));
	}
	if (valeur != NULL) {
		/* On copie la nouvelle valeur. */
		(* (xEA->copier))(valeur, (void **) &(xEA->elements[index]));
	} else {
		xEA->elements[index] = NULL;
	}

	return 0;
}

/* Vérifié. */
void copierXEArray(const void * valeur, void ** lieu) {
	int i;
	xEArray * xEAIn = NULL;
	xEArray * xEAOut = NULL;

	
	xEAIn = (xEArray *) valeur;
	if (xEAIn != NULL) {
		if (xEAIn->elements != NULL) {
			creerXEArray(&xEAOut, xEAIn->nbElements, xEAIn->copier, xEAIn->liberer);
			for (i = 0; i < xEAOut->nbElements; i++) {
				if (xEAIn->elements[i] != NULL) {
					(* (xEAIn->copier))((void *) xEAIn->elements[i], (void **) &(xEAOut->elements[i]));
				} else {
					xEAOut->elements[i] = NULL;
				}
			}
		}
	}
	(* lieu) = xEAOut;
}

/* Vérifié. */
void libererXEArray(void ** lieu) {
	int i;
	xEArray * xEA = NULL;

	xEA = (xEArray *) (* lieu);
	if (xEA != NULL) {
		if (xEA->elements != NULL) {
			for (i = 0; i < xEA->nbElements; i++) {
				(* (xEA->liberer))((void **) &(xEA->elements[i]));
			}
			free(xEA->elements);
			xEA->elements = NULL;
		}
		free(xEA);
		(* lieu) = NULL;
	}
}
