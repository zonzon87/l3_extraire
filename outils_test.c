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

#include "test.h"
#include "outils.h"


int copierCharE_test() {
	int returnValue = 0;
	char c1[] = "Bonjour, comment vas-tu ?";
	void * c2 = NULL;

	copierCharE(c1, &c2);
	PRINT_T(1);
	if (strcmp(c1, c2) != 0) {
		PRINT_T_ERROR();
		returnValue = 1;
	} else {
		PRINT_T_OK();
	}
	libererSimple((void **) &c2);

	return returnValue;
}

int getLine_test(char * fileName) {
	int lastLine = 0;
	FILE * fichier = NULL;
	char * ch = NULL;

	fichier = fopen(fileName, "r");

	if (fichier != NULL)
	{
		printf("### BOF ###\n");
		while (lastLine != LINE_EOF) {
			lastLine = getLine(&ch, fichier);
			printf("%s\n", ch);
			libererSimple((void **) &ch);
		}
		printf("### EOF ###\n");

		fclose(fichier);
	} else {
		P_ERROR_MISSINGFILE(fileName);

		return 1;
	}

	return 0;
}

int removeHeadAndTailChar_test() {
	int returnValue = 0;
	char c1[] = "---Bonjour, comment vas-tu ?-------";
	char c2[] = "-";
	char * cT;

	copierCharE((void *) c1, (void **) &cT);
	removeHeadAndTailChar(&cT, '-');
	PRINT_T(1);
	if (strcmp("Bonjour, comment vas-tu ?", cT) != 0) {
		PRINT_T_ERROR();
		returnValue = 1;
	} else {
		PRINT_T_OK();
	}
	libererSimple((void **) &cT);

	copierCharE((void *) c2, (void **) &cT);
	removeHeadAndTailChar(&cT, '-');
	PRINT_T(2);
	if (strcmp("", cT) != 0) {
		PRINT_T_ERROR();
		returnValue = 1;
	} else {
		PRINT_T_OK();
	}
	libererSimple((void **) &cT);

	return returnValue;
}

int main(void) {
	printf("copierCharE_test() : \n");
	if (copierCharE_test() == 0) {
		PRINT_OK();
	}
	printf("getLine_test(\"Donnees/repas.table\") : \n");
	if (getLine_test("Donnees/repas.table") == 0) {
		printf("Fichier lu, vérifier les données manuellement !\n");
	}
	printf("removeHeadAndTailChar_test() : \n");
	if (removeHeadAndTailChar_test() == 0) {
		PRINT_OK();
	}

	return 0;
}
