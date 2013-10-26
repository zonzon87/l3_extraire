#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "outils.h"


/* Utilisé pour le cas où la ligne termine par |TOKENGARBAGE */
int nbTokenAtFirstLine;


/* Vérifié. */
void destroyTable(void ** tab) {
	if ((* tab) != NULL) {
		file_detruire(&((* ((table **) tab))->lines));
		libererSimple(tab);
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
			c = '\r';
		}
	}
	buffer[length - 1] = '\0';

	(* line) = buffer;

	if (c == EOF) {
		return LINE_EOF;
	}
	return 0;
}

/* Testé. */
int countNumberOfChamps(char * str, const char * delimitor) {
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

	nbTokenAtFirstLine = i;

	/* À cause : du cas où la ligne termine par |TOKENGARBAGE */
	removeHeadAndTailChar(&test, TOKENGARBAGE);
	if (test[0] == '\0') {
		i--;
	}
	libererSimple((void **) &test);

	return i;
}

/* Testé. */
int divideCharEToCharEArray(xEArray ** dest, int nbElements, const char * delimitor, char * src) {
	int i = 0;
	int maxI = 0;
	char * temp = NULL;
	char * token = NULL;
	xEArray * cEA = NULL;

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

/* Testé. */
int rearrangeLineRows(xEArray ** cEAOut, xEArray * cEAIn, const file_parcours values, int nbValues) {
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

/* À vérifier. */
int createTable(table ** tab, const char * fileName, const file ordreApparitions) {
	int returnValue = 0;
	table * tabT = NULL;
	
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

		tabT = (table *) malloc(sizeof (table));
		file_creer(&(tabT->lines), &copierXEArray, &libererXEArray);

		/* Si il y au moins un champ à extraire. */
		if (file_taille(ordreApparitions) > 0) {
			/* TC */
			lastLine = getLine(&line, fichier);
			nbChamps = countNumberOfChamps(line, TOKENDELIMITOR);
			libererSimple((void **) &line);
			tabT->nbRows = nbChamps;

			rewind(fichier);
			/* TC */
			while (lastLine != LINE_EOF) {
				lastLine = getLine(&line, fichier);
				if (line[0] != '\0') { /* Si line est non-vide. */
					result = divideCharEToCharEArray(&cEAIn, nbChamps, TOKENDELIMITOR, line);
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
						file_ajouter(tabT->lines, cEAOut);
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

int createTables(xEArray * tEA, const file nomsTables, const file * tabFChamps) {
	/*int i;
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

		result = createTable(tabx[i], temp, tabFChamps[i]);
		libererSimple((void **) &temp);
		if (result != 0) {
			destroyTables((void **) tabx, nbFichiers);
		}
		i++;
	}
	file_parcours_detruire(&parcours);
	*/
	return 0;
}

void charEArrayToPrint(xEArray * cEA) {
	int i;

	for (i = 0; i < cEA->nbElements; i++) {
		printf("%s|", (char *) accesXEArray(cEA, i));
	}
	printf("\n");
}

void tableToPrint(table * tab) {
	file_parcours parcours = file_parcours_creer(tab->lines);
	xEArray * cEA = NULL;

	while (!file_parcours_est_fini(parcours)) {
		file_parcours_suivant(parcours, (void **) &cEA);
		charEArrayToPrint(cEA);
		libererXEArray((void **) &cEA);
	}
	file_parcours_detruire(&parcours);
}
