/* gcc -ansi -Wall -Wextra -pedantic -ggdb -lm -o requete_test file.o requete.o requete_test.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "requete.h"

int copierCharEtoile_test() {
	char c1[] = "Bonjour";
	void * c2;

	copierCharEtoile(c1, &c2);

	c2 = (char *) c2;
	if (strcmp(c1, c2) != 0) {
		return 1;
	}

	free(c2);

	return 0;
}

int parseSyntaxChamp_test() {
	int fileNumber;
	int rowNumber;

	/* Les tests suivants doivent être corrects. */
	if ((parseSyntaxChamp('.', "abc.77", &fileNumber, &rowNumber) != 0) || (fileNumber != 730) || (rowNumber != 76)) {
		return 1;
	}
	if ((parseSyntaxChamp('.', "aa.123", &fileNumber, &rowNumber) != 0) || (fileNumber != 26) || (rowNumber != 122)) {
		return 2;
	}
	if ((parseSyntaxChamp('.', "a.1", &fileNumber, &rowNumber) != 0) || (fileNumber != 0) || (rowNumber != 0)) {
		return 3;
	}
	if ((parseSyntaxChamp('-', "abc-77", &fileNumber, &rowNumber) != 0) || (fileNumber != 730) || (rowNumber != 76)) {
		return 4;
	}

	/* Les tests suivants doivent être incorrects. */
	if (parseSyntaxChamp('.', "abc.0", &fileNumber, &rowNumber) == 0) {
		return 5;
	}
	if (parseSyntaxChamp('.', ".77", &fileNumber, &rowNumber) == 0) {
		return 6;
	}
	if (parseSyntaxChamp('.', "abc.", &fileNumber, &rowNumber) == 0) {
		return 7;
	}
	if (parseSyntaxChamp('.', "abc77", &fileNumber, &rowNumber) == 0) {
		return 8;
	}
	if (parseSyntaxChamp('.', "ab0c.77", &fileNumber, &rowNumber) == 0) {
		return 9;
	}
	if (parseSyntaxChamp('.', "abc.77x", &fileNumber, &rowNumber) == 0) {
		return 10;
	}
	if (parseSyntaxChamp('.', "abc.7y7", &fileNumber, &rowNumber) == 0) {
		return 11;
	}

	return 0;
}

int main(void) {
	int result;

	printf("copierCharEtoile_test : ");
	result = copierCharEtoile_test();
	if (result == 0) {
		printf("OK !\n");
	} else {
		printf("AT TEST %d ERROR", result);
	}

	printf("parseSyntaxChamp_test : ");
	result = parseSyntaxChamp_test();
	if (result == 0) {
		printf("OK !\n");
	} else {
		printf("AT TEST %d ERROR", result);
	}

	return 0;
}
