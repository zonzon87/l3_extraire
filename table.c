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

int countNumberOfChamps(char * ch) {
/* Attention strtok() pourrit la chaîne passée en paramètre. */
	int count = 0;
	char * copy;
	char * token;

	copierCharEtoile(ch, (void **) &copy);

	token = strtok(ch, TOKENDELIMITOR);
	while (token != NULL) {
		count++;
		token = strtok (NULL, TOKENDELIMITOR);
	}

	return count;
}

int divideCharEtoiletoCharEtoileArray(char ** dest, int nbElements, char * src) {

	return 0;
}

int copyTo() {

	return 0;
}

int createTable(table ** tab, char * fileName, ordreApparition * ord) {
	FILE * fichier = NULL;

	fichier = fopen(fileName, "r");

	if (fichier != NULL) {
		int nbChamps;
		int lastLine = 0;
		char * line = NULL;
		char ** tabChampsIn;
		char ** tabChampsOut;

		(* tab) = (table *) malloc(sizeof (table));
		file_creer(&((* tab)->lines), &copierCharEtoileArray, &libererCharEtoileArray);

		/* TC */
		lastLine = getLine(&line, fichier);
		nbChamps = countNumberOfChamps(line);
		/* TC */
		if (nbChamps < (ord->maxValue)) {
			P_ERROR_INEXISTANTCHAMP(fileName, ord->maxValue);

			destroyTable(tab);
			libererSimple((void **) &line);

			return ERROR_INEXISTANTCHAMP;
		}
		divideCharEtoiletoCharEtoileArray((char **) &tabChampsIn, nbChamps, line);
		libererSimple((void **) &line);
		
		/* TC */
		while (lastLine != LINE_EOF) {
			lastLine = getLine(&line, fichier);

			libererSimple((void **) &line);
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