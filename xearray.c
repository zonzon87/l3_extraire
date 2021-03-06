/*****************************************************************************
 *                                                                           *
 *   Copyright © 2013: Alexis Lavie, Hugo Thibaud.                           *
 *                                                                           *
 *   This file is licensed under the GPLv3: General Public License v3.       *
 *                                                                           *
 *****************************************************************************/

#include <stdlib.h>

#include "xearray.h"


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

void * accesXEArray(xEArray * xEA, int index) {
	if ((index > -1) && (index < xEA->nbElements)) {
		return xEA->elements[index];
	} else {
		return NULL;
	}
}

int ajouterXEArray(xEArray * xEA, int index, const void * valeur) {
	if ((xEA == NULL) || (xEA->elements == NULL)) {
		return XEARRAY_NOT_A_EARRAY;
	}

	/* Si l'index est erroné. */
	if ((index < 0) || (index > (xEA->nbElements - 1))) {
		return XEARRAY_WRONGINDEX;
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

int ajouterLienXEArray(xEArray * xEA, int index, void * valeur) {
	if ((xEA == NULL) || (xEA->elements == NULL)) {
		return XEARRAY_NOT_A_EARRAY;
	}

	/* Si l'index est erroné. */
	if ((index < 0) || (index > (xEA->nbElements - 1))) {
		return XEARRAY_WRONGINDEX;
	}

	if (valeur != NULL) {
		/* On lie la nouvelle valeur. */
		xEA->elements[index] = valeur;
	} else {
		xEA->elements[index] = NULL;
	}

	return 0;
}


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

void libererLienXEArray(void ** lieu) {
	xEArray * xEA = NULL;

	xEA = (xEArray *) (* lieu);
	if (xEA != NULL) {
		if (xEA->elements != NULL) {
			free(xEA->elements);
			xEA->elements = NULL;
		}
		free(xEA);
		(* lieu) = NULL;
	}
}
