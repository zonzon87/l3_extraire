#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "outils.h"


void destroyTable(table ** tab) {
	if ((* tab) != NULL) {
		file_detruire(&((* tab)->lines));
		libererSimple((void **) tab);
	}
}

/* truc bizarre si mac os dernière ligne vide est ignorée. */
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

/* Attention strtok() pourrit la chaîne passée en paramètre. */
int countNumberOfChamps(const char * ch, const char * delimitor) {
	int count = 0;
	char * copy = NULL;
	char * token = NULL;

	copierCharEtoile((void *) ch, (void **) &copy);

	token = strtok(copy, delimitor);
	while (token != NULL) {
		count++;
		token = strtok(NULL, delimitor);
	}

	return count;
}

int divideCharEtoiletoCharEtoileArray(charEtoileArray ** dest, int nbElements, const char * delimitor, char * src) {
	int i = 0;
	char * token = NULL;
	charEtoileArray * cEA = NULL;

	creerCharEtoileArray(&cEA, nbElements);

	token = strtok(src, delimitor);
	while (token != NULL) {
		if (i < nbElements) { 
			copierCharEtoile((void *) token, (void **) &(cEA->chs[i]));
			removeHeadAndTailChar(&(cEA->chs[i]), TOKENGARBAGE);
		} else {
			libererCharEtoileArray((void **) &cEA);
			return ERROR_MALFORMEDFILE;
		}
		i++;
		token = strtok(NULL, delimitor);
	}

	if (i != nbElements) {
		libererCharEtoileArray((void **) &cEA);
		return ERROR_MALFORMEDFILE;
	}

	(* dest) = cEA;

	return 0;
}

int rearrangeTableRows(charEtoileArray ** cEAOut, charEtoileArray * cEAIn, file_parcours * values, int nbValues) {

	return 0;
}

int createTable(table ** tab, const char * fileName, file ordreApparitions, int maxValue) {
	FILE * fichier = NULL;

	fichier = fopen(fileName, "r");

	if (fichier != NULL) {
		int nbChamps;
		int result;
		int lastLine = 0;
		char * line = NULL;
		charEtoileArray * tabChampsIn = NULL;
		charEtoileArray * tabChampsOut = NULL;

		(* tab) = (table *) malloc(sizeof (table));
		file_creer(&((* tab)->lines), &copierCharEtoileArray, &libererCharEtoileArray);

		/* Si il y au moins un champ à extraire. */
		if (file_taille(ordreApparitions) > 0) {
			/* TC */
			lastLine = getLine(&line, fichier);
			nbChamps = countNumberOfChamps(line, TOKENDELIMITOR);
			/* TC */
			if (nbChamps < (maxValue)) {
				P_ERROR_INEXISTANTCHAMP(fileName, maxValue);

				destroyTable(tab);
				libererSimple((void **) &line);

				return ERROR_INEXISTANTCHAMP;
			}
			result = divideCharEtoiletoCharEtoileArray(&tabChampsIn, nbChamps, TOKENDELIMITOR, line);
			libererSimple((void **) &line);
			if (result != 0) {
				if (result == ERROR_MALFORMEDFILE) {
					P_ERROR_MALFORMEDFILE(fileName);
				}

				return result;
			}

			/* Modif vers tabChampsOut */
			/* free tabChampsIn */
			/* ajout à tab */
			/* free tabChampsOut */

			/* TC */
			while (lastLine != LINE_EOF) {
				lastLine = getLine(&line, fichier);

				/* A COMPLETER */

				libererSimple((void **) &line);
			}
		}


		fclose(fichier);
	} else {
		P_ERROR_MISSINGFILE(fileName);

		return ERROR_MISSINGFILE;
	}

	return 0;
}

int createTables(file * tables, requete * reqIn, requete ** reqOut) {

	return 0;
}