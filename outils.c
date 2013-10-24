#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "outils.h"


/* Vérifié. */
void libererSimple(void ** lieu) {
	free(* lieu);
	(* lieu) = NULL;
}

/* Vérifié. */
void copierIntEtoile(const void * valeur, void ** lieu) {
	(* lieu) = (void *) malloc (sizeof (int));
	(** ((int **) lieu)) = (* ((int *) valeur));
}

/* Vérifié */
void copierCharEtoile(const void * valeur, void ** lieu) {
	(* lieu) = (char *) malloc((sizeof (char)) * (strlen((char *) valeur) + 1));
	strcpy((* lieu), valeur);
}

/* Vérifié. */
void creerCharEtoileArray(charEtoileArray ** cEA, int nbElements) {
	int i;

	(* cEA) = (charEtoileArray *) malloc(sizeof (charEtoileArray));
	(* cEA)->nbChs = nbElements;
	(* cEA)->chs = (char **) malloc((sizeof (char *)) * nbElements);
	for (i = 0; i < nbElements; i++) {
		(* cEA)->chs[i] = NULL;
	}
}

/* Vérifié. */
void copierCharEtoileArray(const void * valeur, void ** lieu) {
	int i;
	charEtoileArray * cEAIn = NULL;
	charEtoileArray * cEAOut = NULL;

	cEAIn = (charEtoileArray *) valeur;
	creerCharEtoileArray(&cEAOut, cEAIn->nbChs);
	for (i = 0; i < cEAOut->nbChs; i++) {
		copierCharEtoile((void *) cEAIn->chs[i], (void **) &(cEAOut->chs[i]));
	}
	(* lieu) = cEAOut;
}

/* Vérifié. */
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

/* Vérifié. */
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

/* Vérifié. */
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

/* Vérifié. */
int getLine(char ** line, FILE * fichier) {
	int bufferLength = 0;
	int bufferCount = 0;
	int length = 0;
	char c = '\0';
	char * buffer = NULL;

	while ((c != '\r') && (c != '\n') && (c != EOF)) {
		if(bufferCount == bufferLength) {
			bufferCount = 0;
			bufferLength += TABLE_BUFFER;
			buffer = (char *) realloc(buffer, bufferLength);
		}
		c = getc(fichier);
		buffer[length] = c;
		length++;
		bufferCount++;
	}
	if (c == '\r') {
		c = getc(fichier);
		if (c != '\n') {
			fseek(fichier, -1, SEEK_CUR);
		}
	}
	buffer[length - 1] = '\0';

	(* line) = buffer;

	if (c == EOF) {
		return LINE_EOF;
	}
	return 0;
}

/* Vérifié. */
void removeHeadAndTailChar(char ** str, char c) {
	int i;
	int length = (int) strlen(* str);
	int start = - 1;
	int end = - 1;
	char * newCh = NULL;

	for (i = 0; i < length; i++) {
		if ((* str)[i] != c) {
			if (start < 0) {
				start = i;
				end = start;
			} else {
				end = i;
			}
		}
	}
	if (!((start < 0) && (end < 0))) {
		{
			int length = end - start + 1;

			newCh = (char *) malloc((sizeof (char)) * (length + 1));
			memcpy(newCh, (* str) + start, (sizeof (char)) * length);
			newCh[length] = '\0';
		}
	} else {
		newCh = (char *) malloc(sizeof (char));
		newCh[0] = '\0';
	}

	libererSimple((void **) str);
	(* str) = newCh;
}
