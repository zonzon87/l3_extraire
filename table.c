/*****************************************************************************
 *                                                                           *
 *   Copyright © 2013: Alexis Lavie, Hugo Thibaud.                           *
 *                                                                           *
 *   This file is licensed under the GPLv3: General Public License v3.       *
 *                                                                           *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "outils.h"


void destroyTable(void ** tab) {
    file_detruire((file *) tab);
}

void destroyTables(tables ** tEA) {
	libererXEArray((void **) tEA);
}

int countNumberOfChamps(char * str, const char * delimitor, int * nbTokenAtFirstLine) {
	int i = 0;
	char * test = NULL;
	char * token = NULL;

	token = strtok(str, delimitor);
	while (token != NULL) {
		i++;
		libererSimple((void **) &test);
		copierCharE((void *) token, (void **) &test);
		token = strtok(NULL, delimitor);
	}

	* nbTokenAtFirstLine = i;

	/* À cause : du cas où la ligne termine par |TOKENGARBAGE */
	removeHeadAndTailChar(&test, TOKENGARBAGE);
	if (test[0] == '\0') {
		i--;
	}
	libererSimple((void **) &test);

	return i;
}

int divideCharEToCharEArray(tableLine ** dest, int nbElements, int nbTokenAtFirstLine, const char * delimitor, char * src) {
	int i = 0;
	int maxI = 0;
	char * temp = NULL;
	char * token = NULL;
	tableLine * cEA = NULL;

	creerXEArray(&cEA, nbElements, &copierCharE, &libererSimple);

	token = strtok(src, delimitor);
	while (token != NULL) {
		if (i < nbElements + 1) { /* + 1 à cause : du cas où la ligne termine par |TOKENGARBAGE */
			copierCharE((void *) token, (void **) &temp);
			removeHeadAndTailChar(&temp, TOKENGARBAGE);
			if ((i == nbElements) && (temp[0] == '\0')) { /* encore à cause : du cas où la ligne termine par |TOKENGARBAGE */
				i--;
			} else {
				ajouterXEArray(cEA, i, (void *) temp);
			}
			libererSimple((void **) &temp);
		} else {
			libererXEArray((void **) &cEA);
			return ERROR_MALFORMEDFILE;
		}

		token = strtok(NULL, delimitor);
		i++;
		maxI++;
	}

	if ((i != nbElements) || (nbTokenAtFirstLine != maxI)) {
		libererXEArray((void **) &cEA);
		return ERROR_MALFORMEDFILE;
	}

	(* dest) = cEA;
	return 0;
}

int rearrangeLineRows(tableLine ** cEAOut, tableLine * cEAIn, const file_parcours values, int nbValues) {
	int i = 0;
	int * value = NULL;

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

int createTable(table ** tab, const char * fileName, const file ordreApparitions) {
	int returnValue = 0;
	table * tabT = NULL;

	FILE * fichier = NULL;
	fichier = fopen(fileName, "r");

	if (fichier != NULL) {
		int nbChamps;
		int nbTokenAtFirstLine;
		int result;
		int lastLine = 0;
		char * line = NULL;
		file_parcours values = NULL;
		tableLine * cEAIn = NULL;
		tableLine * cEAOut = NULL;

		file_creer(&tabT, &copierXEArray, &libererXEArray);

		/* Si il y au moins un champ à extraire. */
		if (file_taille(ordreApparitions) > 0) {
			/* TC */
			lastLine = getLine(&line, fichier);
			nbChamps = countNumberOfChamps(line, TOKENDELIMITOR, &nbTokenAtFirstLine);
			libererSimple((void **) &line);

			rewind(fichier);
			/* TC */
			while (lastLine != LINE_EOF) {
				lastLine = getLine(&line, fichier);
				if (line[0] != '\0') { /* Si line est non-vide. */
					result = divideCharEToCharEArray(&cEAIn, nbChamps, nbTokenAtFirstLine, TOKENDELIMITOR, line);
					libererSimple((void **) &line);

					if (result != 0) {
						if (result == ERROR_MALFORMEDFILE) {
							P_ERROR_MALFORMEDFILE(fileName);
						}

						libererXEArray((void **) &cEAIn);
						returnValue = result;
						break;
					}

					values = file_parcours_creer(ordreApparitions);
					result = rearrangeLineRows(&cEAOut, cEAIn, values, file_taille(ordreApparitions));

					if (result == 0) {
						file_ajouter(tabT, cEAOut);
					}

					file_parcours_detruire(&values);
					libererXEArray((void **) &cEAOut);

					if (result != 0) {
						if (result == ERROR_INEXISTANTCHAMP) {
							P_ERROR_INEXISTANTCHAMP(fileName, cEAIn->nbElements);
						}

						libererXEArray((void **) &cEAIn);
						returnValue = result;
						break;
					}

					libererXEArray((void **) &cEAIn);
				} else {
					libererSimple((void **) &line);
				}
			}
		}
		fclose(fichier);
	} else {
		P_ERROR_MISSINGFILE(fileName);

		returnValue = ERROR_MISSINGFILE;
	}

	if (returnValue != 0) {
		destroyTable((void **) &tabT);
	} else {
		(* tab) = tabT;
	}
	return returnValue;
}

int createTables(tables ** tEA, const file nomsFichiers, const file * ordreApparitions) {
	int i = 0;
	int result;
	char * nomFichier = NULL;
	file_parcours parcours = NULL;
	table * tab = NULL;
	tables * tEAT = NULL;

	creerXEArray(&tEAT, file_taille(nomsFichiers), NULL, &destroyTable);

	parcours = file_parcours_creer(nomsFichiers);
	while (!file_parcours_est_fini(parcours)) {
		file_parcours_suivant(parcours, (void **) &nomFichier);
		result = createTable(&tab, nomFichier, ordreApparitions[i]);
		libererSimple((void **) &nomFichier);
		if (result != 0) {
			destroyTables(&tEAT);
			file_parcours_detruire(&parcours);
			return result;
		} else {
			ajouterLienXEArray(tEAT, i, (void *) tab);
			tab = NULL;
		}
		i++;
	}
	file_parcours_detruire(&parcours);

	(* tEA) = tEAT;
	return 0;
}

void tableLineToPrint(tableLine * cEA) {
	int i;

	for (i = 0; i < cEA->nbElements; i++) {
		printf("%s|", (char *) accesXEArray(cEA, i));
	}
	printf("\n");
}

void tableToPrint(table * tab) {
	file_parcours parcours = file_parcours_creer(tab);
	tableLine * cEA = NULL;

	while (!file_parcours_est_fini(parcours)) {
		file_parcours_suivant(parcours, (void **) &cEA);
		tableLineToPrint(cEA);
		libererXEArray((void **) &cEA);
	}
	file_parcours_detruire(&parcours);
}
