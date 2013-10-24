#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"


/* Vérifié. */
void destroyTable(table ** tab) {
	if ((* tab) != NULL) {
		file_detruire(&((* tab)->lines));
		libererSimple((void **) tab);
	}
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
int countNumberOfChamps(char * str, const char * delimitor) {
	int i = 0;
	char * token = NULL;

	token = strtok(str, delimitor);
	while (token != NULL) {
		i++;
		token = strtok(NULL, delimitor);
	}

	return i;
}

/* Vérifié. */
int divideCharEToCharEArray(xEArray ** dest, int nbElements, const char * delimitor, char * src) {
	int i = 0;
	char * temp;
	char * token = NULL;
	xEArray * cEA = NULL;

	creerXEArray(&cEA, nbElements, &copierCharE, &libererSimple);

	token = strtok(src, delimitor);
	while (token != NULL) {
		if (i < nbElements) {
			ajouterXEArray(cEA, i, (void *) token);
			temp = (char *) accesXEArray(cEA, i);
			removeHeadAndTailChar(&temp, TOKENGARBAGE);
		} else {
			libererXEArray((void **) &cEA);
			return ERROR_MALFORMEDFILE;
		}
		i++;
		token = strtok(NULL, delimitor);
	}

	if (i != nbElements) {
		libererXEArray((void **) &cEA);
		return ERROR_MALFORMEDFILE;
	}

	(* dest) = cEA;

	return 0;
}

/* Vérifié. */
int rearrangeLineRows(xEArray ** cEAOut, xEArray * cEAIn, file_parcours values, int nbValues) {
	int i = 0;
	int * value;

	creerXEArray(cEAOut, nbValues, &copierCharE, &libererSimple);
	while (!file_parcours_est_fini(values)) {
		file_parcours_suivant(values, (void **) &value);
		ajouterXEArray(* cEAOut, i, accesXEArray(cEAIn, * value));
		libererSimple((void **) &value);
		i++;
	}

	return 0;
}

/* À vérifier. */
int createTable(table ** tab, const char * fileName, file ordreApparitions, int maxValue) {
	FILE * fichier = NULL;

	fichier = fopen(fileName, "r");

	if (fichier != NULL) {
		int nbChamps;
		int result;
		int lastLine = 0;
		char * line = NULL;
		file_parcours values = NULL;
		xEArray * cEAIn = NULL;
		xEArray * cEAOut = NULL;

		(* tab) = (table *) malloc(sizeof (table));
		file_creer(&((* tab)->lines), &copierXEArray, &libererXEArray);

		/* Si il y au moins un champ à extraire. */
		if (file_taille(ordreApparitions) > 0) {
			/* TC */
			lastLine = getLine(&line, fichier);
			nbChamps = countNumberOfChamps(line, TOKENDELIMITOR);
			libererSimple((void **) &line);
			if (nbChamps < (maxValue)) {
				P_ERROR_INEXISTANTCHAMP(fileName, maxValue);

				destroyTable(tab);
				fclose(fichier);
				return ERROR_INEXISTANTCHAMP;
			}

			/* TC */
			rewind(fichier);
			while (lastLine != LINE_EOF) {
				lastLine = getLine(&line, fichier);
				result = divideCharEToCharEArray(&cEAIn, nbChamps, TOKENDELIMITOR, line);
				libererSimple((void **) &line);

				if (result != 0) {
					if (result == ERROR_MALFORMEDFILE) {
						P_ERROR_MALFORMEDFILE(fileName);
					}

					destroyTable(tab);
					libererXEArray((void **) &cEAIn);
					fclose(fichier);
					return result;
				}

				values = file_parcours_creer(ordreApparitions);
				rearrangeLineRows(&cEAOut, cEAIn, values, file_taille(ordreApparitions));
				file_parcours_detruire(&values);
				file_ajouter((* tab)->lines, cEAOut);
				libererXEArray((void **) &cEAOut);
			}
		}

		fclose(fichier);
	} else {
		P_ERROR_MISSINGFILE(fileName);

		return ERROR_MISSINGFILE;
	}

	return 0;
}

int createTables(table ** tables, file * tabFChamps, int nbFichiers) {

	
	return 0;
}
