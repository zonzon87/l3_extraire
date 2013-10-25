#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"


/* Vérifié. */
void destroyTable(void ** tab) {
	table * t = (table *) (* tab);
	if (t != NULL) {
		file_detruire(&(t->lines));
		libererSimple((void **) &t);
	}
}

void newTables(void ** tabs, int nbTables) {
	table ** ts = NULL;
	ts = (table **) malloc((sizeof (table *)) * nbTables);
	(* tabs) = ts;
}

void destroyTables(void ** tabs, int nbTables) {
	table ** ts = (table **) (* tabs);
	if(ts != NULL) {
		int i;

		for (i = 0; i < nbTables; i++){
			destroyTable((void **) &(ts[i]));
		}
		libererSimple((void **) ts);
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
	char * temp = NULL;
	char * token = NULL;
	xEArray * cEA = NULL;

	creerXEArray(&cEA, nbElements, &copierCharE, &libererSimple);

	token = strtok(src, delimitor);
	while (token != NULL) {
		if (i < nbElements) {
			copierCharE((void *) token, (void **) &temp);
			removeHeadAndTailChar(&temp, TOKENGARBAGE);
			ajouterXEArray(cEA, i, (void *) temp);
			libererSimple((void **) &temp);
		} else {
			libererXEArray((void **) &cEA);
			return ERROR_MALFORMEDFILE;
		}

		token = strtok(NULL, delimitor);
		i++;
	}

	if (i != nbElements) {
		libererXEArray((void **) &cEA);
		return ERROR_MALFORMEDFILE;
	}

	(* dest) = cEA;
	return 0;
}

/* Vérifié. */
int rearrangeLineRows(xEArray ** cEAOut, xEArray * cEAIn, const file_parcours values, int nbValues) {
	int i = 0;
	int * value;

	creerXEArray(cEAOut, nbValues, &copierCharE, &libererSimple);
	while (!file_parcours_est_fini(values)) {
		file_parcours_suivant(values, (void **) &value);
		if (((* value) + 1) > cEAIn->nbElements) {

			libererSimple((void **) &value);
			libererXEArray((void **) cEAOut);
			return ERROR_INEXISTANTCHAMP;
		}
		ajouterXEArray(* cEAOut, i, accesXEArray(cEAIn, * value));
		libererSimple((void **) &value);
		i++;
	}

	return 0;
}

/* À vérifier. */
int createTable(table * tab, const char * fileName, const file ordreApparitions) {
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

		file_creer(&(tab->lines), &copierXEArray, &libererXEArray);

		/* Si il y au moins un champ à extraire. */
		if (file_taille(ordreApparitions) > 0) {
			/* TC */
			lastLine = getLine(&line, fichier);
			nbChamps = countNumberOfChamps(line, TOKENDELIMITOR);
			libererSimple((void **) &line);
			tab->nbRows = nbChamps;

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

					libererXEArray((void **) &cEAIn);
					fclose(fichier);
					return result;
				}

				values = file_parcours_creer(ordreApparitions);
				result = rearrangeLineRows(&cEAOut, cEAIn, values, file_taille(ordreApparitions));
				if (result == 0) {
					file_ajouter(tab->lines, cEAOut);
				}
				file_parcours_detruire(&values);
				libererXEArray((void **) &cEAOut);
				if (result != 0) {
					if (result == ERROR_INEXISTANTCHAMP) {
						P_ERROR_INEXISTANTCHAMP(fileName, cEAIn->nbElements);
					}
					fclose(fichier);
					return result;
				}
				libererXEArray((void **) &cEAIn);
			}
		}

		fclose(fichier);
	} else {
		P_ERROR_MISSINGFILE(fileName);

		return ERROR_MISSINGFILE;
	}

	return 0;
}

int createTables(void ** tabs, const file nomsTables, const file * tabFChamps) {
	int i;
	int nbFichiers;
	int result;
	table ** tabx = NULL;
	char * temp;
	file_parcours parcours = NULL;

	i = 0;
	nbFichiers = file_taille(nomsTables);
	newTables((void **) &tabx, nbFichiers);

	parcours = file_parcours_creer(nomsTables);
	while (!file_parcours_est_fini(parcours)) {
		file_parcours_suivant(parcours, (void **) &temp);
		tabx[i] = (table *) malloc(sizeof (table));
		result = createTable(tabx[i], temp, tabFChamps[i]);
		libererSimple((void **) &temp);
		if (result != 0) {
			destroyTables((void **) tabx, nbFichiers);
		}
		i++;
	}
	file_parcours_detruire(&parcours);

	return 0;
}

void tableToPrint(table * tab) {

}
