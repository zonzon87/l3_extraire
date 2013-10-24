#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "outils.h"


/* Vérifié. */
void destroyTable(table ** tab) {
	if ((* tab) != NULL) {
		file_detruire(&((* tab)->lines));
		libererSimple((void **) tab);
	}
}

/* Vérifié. */
int countNumberOfChamps(const char * str, const char * delimitor) {
	int i = 0;
	char * copy = NULL;
	char * token = NULL;

	/* strtok() peut modifier bizarrement la chaîne passée en paramètre. */
	copierCharEtoile((void *) str, (void **) &copy);

	token = strtok(copy, delimitor);
	while (token != NULL) {
		i++;
		token = strtok(NULL, delimitor);
	}

	return i;
}

/* Vérifié. */
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

/* Vérifié. */
int rearrangeLineRows(charEtoileArray ** cEAOut, charEtoileArray * cEAIn, file_parcours values, int nbValues) {
	int i = 0;
	int * value;

	creerCharEtoileArray(cEAOut, nbValues);
	while (!file_parcours_est_fini(values)) {
		file_parcours_suivant(values, (void **) &value);
		copierCharEtoile((void *) (cEAIn->chs[* value]), (void **) &((* cEAOut)->chs[i]));
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
		charEtoileArray * cEAIn = NULL;
		charEtoileArray * cEAOut = NULL;

		(* tab) = (table *) malloc(sizeof (table));
		file_creer(&((* tab)->lines), &copierCharEtoileArray, &libererCharEtoileArray);

		/* Si il y au moins un champ à extraire. */
		if (file_taille(ordreApparitions) > 0) {
			/* TC */
			lastLine = getLine(&line, fichier);
			nbChamps = countNumberOfChamps(line, TOKENDELIMITOR);
			if (nbChamps < (maxValue)) {
				P_ERROR_INEXISTANTCHAMP(fileName, maxValue);

				destroyTable(tab);
				libererSimple((void **) &line);
				fclose(fichier);
				return ERROR_INEXISTANTCHAMP;
			}

			/* TC */
			rewind(fichier);
			while (lastLine != LINE_EOF) {
				lastLine = getLine(&line, fichier);

				result = divideCharEtoiletoCharEtoileArray(&cEAIn, nbChamps, TOKENDELIMITOR, line);
				libererSimple((void **) &line);

				if (result != 0) {
					if (result == ERROR_MALFORMEDFILE) {
						P_ERROR_MALFORMEDFILE(fileName);
					}

					destroyTable(tab);
					libererCharEtoileArray((void **) &cEAIn);
					fclose(fichier);
					return result;
				}

				values = file_parcours_creer(ordreApparitions);
				rearrangeLineRows(&cEAOut, cEAIn, values, file_taille(ordreApparitions));
				file_parcours_detruire(&values);
				file_ajouter((* tab)->lines, cEAOut);
				libererCharEtoileArray((void **) &cEAOut);
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