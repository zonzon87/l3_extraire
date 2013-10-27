/*****************************************************************************
 *                                                                           *
 *   Copyright © 2013, Alexis Lavie, Hugo Thibaud.                           *
 *                                                                           *
 *   This file is licensed under the GPLv3: General Public License v3.       *
 *                                                                           *
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "outils.h"


void libererSimple(void ** lieu) {
	free(* lieu);
	(* lieu) = NULL;
}

void copierIntE(const void * valeur, void ** lieu) {
	(* lieu) = (void *) malloc (sizeof (int));
	(** ((int **) lieu)) = (* ((int *) valeur));
}

void copierCharE(const void * valeur, void ** lieu) {
	(* lieu) = (char *) malloc((sizeof (char)) * (strlen((char *) valeur) + 1));
	strcpy((* lieu), valeur);
}

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

void removeHeadAndTailChar(char ** str, char c) {
	int i;
	int length = (int) strlen(* str);
	int start = -1;
	int end = -1;
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
