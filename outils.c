#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "outils.h"


void libererSimple(void ** lieu) {
	free(* lieu);
	(* lieu) = NULL;
}

void copierCharEtoile(const void * valeur, void ** lieu) {
	(* lieu) = (char *) malloc((sizeof (char)) * (strlen((char *) valeur) + 1));
	strcpy((* lieu), valeur);
}

void copierCharEtoileArray(const void * valeur, void ** lieu) {
	int i;
	charEtoileArray * cEAIn = NULL;
	charEtoileArray * cEAOut = NULL;

	cEAIn = (charEtoileArray *) valeur;
	cEAOut = (charEtoileArray *) malloc(sizeof (charEtoileArray));
	cEAOut->nbChs = cEAIn->nbChs;
	cEAOut->chs = (char **) malloc((sizeof (char *)) * cEAOut->nbChs);

	for (i = 0; i < cEAOut->nbChs; i++) {
		copierCharEtoile((void *) cEAIn->chs[i], (void **) &(cEAOut->chs[i]));
	}
	(* lieu) = cEAOut;
}

void libererCharEtoileArray(void ** lieu) {
	if ((* lieu) != NULL) {
		{
			int i;
			charEtoileArray * cEA = NULL;
			
			cEA = (charEtoileArray *) (* lieu);
			for (i = 0; i < cEA->nbChs; i++) {
				libererSimple((void **) &(cEA->chs[i]));
			}
			libererSimple((void **) &(cEA->chs));
			libererSimple((void **) &(cEA));
		}
	}
}

int isInVAList(char c, int argc, ...) {
	int i;
	va_list argList;

	va_start(argList, argc);

	for (i = 0; i < argc; i++) {
		if (c == (char) (va_arg(argList, int))) {
			va_end(argList);
			return 0;
		}
	}

	va_end(argList);
	return 1;
}

int isNumeric(const char * str, const int strLength) {
	int i;
	int temp;

	for (i = 0; i < strLength; i++) {
		temp = (int) str[i];
		if ((temp < 48) || (temp > 57)) { /* Si le caractère n'est pas compris entre '0' et '9' inclus, bref : n'est pas un chiffre. */
			return 1;
		}
	}

	return 0;
}
