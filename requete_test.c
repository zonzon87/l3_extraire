/* gcc -ansi -Wall -Wextra -pedantic -ggdb -lm -o requete_test file.o outils.o requete.o requete_test.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "requete.h"
#include "outils.h"

#define PRINT_ERROR(x) printf("\tTest %d Failed !\n", x);

int parseSyntaxChamp_test() {
	int i;
	int returnValue = 0;
	champ * ch = NULL;

	/* Les tests suivants doivent être corrects. */
	char * c1[] = {".", "abc.77", "730", "76"};
	char * c2[] = {".", "aa.123", "26", "122"};
	char * c3[] = {".", "a.1", "0", "0"};
	char * c4[] = {"-", "abc-77", "730", "76"};

	/* Les tests suivants doivent être incorrects. */
	char * c5[] = {".", "abc.0"};
	char * c6[] = {".", ".77"};
	char * c7[] = {".", "abc."};
	char * c8[] = {".", "abc77"};
	char * c9[] = {".", "ab0c.77"};
	char * c10[] = {".", "abc.77x"};
	char * c11[] = {".", "abc.7y7"};
	char * c12[] = {".", "abc-77"};
	
	char ** c[12];
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
	c[3] = c4;
	c[4] = c5;
	c[5] = c6;
	c[6] = c7;
	c[7] = c8;
	c[8] = c9;
	c[9] = c10;
	c[10] = c11;
	c[11] = c12;

	for (i = 0; i < 4; i++) {
		if ((parseSyntaxChamp(&ch, c[i][0][0], c[i][1]) != 0) || (ch->table != atoi(c[i][2])) || (ch->row != atoi(c[i][3]))) {
			PRINT_ERROR(i + 1);
			returnValue = 1;
		}
		libererSimple((void **) &ch);
	}
	for (i = 4; i < 12; i++) {
		if (parseSyntaxChamp(&ch, c[i][0][0], c[i][1]) == 0) {
			PRINT_ERROR(i + 1);
			returnValue = 1;
		}
		libererSimple((void **) &ch);/* par sécurité */
	}

	return returnValue;
}

int parseSyntaxCondition_test() {
	int i;
	int returnValue = 0;
	condition * co = NULL;


	/* Les tests suivants doivent être corrects. */
	char * c1[] = {".", "abc.77<aa.123", "730", "76", "<", "26", "122"};
	char * c2[] = {".", "abc.77<=aa.123", "730", "76", "<=", "26", "122"};
	char * c3[] = {".", "abc.77<.aa.123", "730", "76", "<.", "26", "122"};
	char * c4[] = {".", "abc.77<=.aa.123", "730", "76", "<=.", "26", "122"};
	char * c5[] = {".", "abc.77=aa.123", "730", "76", "=", "26", "122"};
	char * c6[] = {".", "abc.77!=aa.123", "730", "76", "!=", "26", "122"};

	/* Les tests suivants doivent être incorrects. */
	char * c7[] = {".", "abc.77.<aa.123"};
	char * c8[] = {".", "abc.77.aa.123"};
	char * c9[] = {".", "abc.77=!aa.123"};
	char * c10[] = {".", "abc.77aa<.123"};
	char * c11[] = {".", "abc<77aa.123"};
	char * c12[] = {".", "abc.77<>aa.123"};
	char * c13[] = {".", "abc.77aa.123"};
	char * c14[] = {".", "abc.77aa.123<"};
	char * c15[] = {".", ">abc.77aa.123"};
	char * c16[] = {".", "abc.77=a.a.123"};

	char ** c[16];
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
	c[3] = c4;
	c[4] = c5;
	c[5] = c6;
	c[6] = c7;
	c[7] = c8;
	c[8] = c9;
	c[9] = c10;
	c[10] = c11;
	c[11] = c12;
	c[12] = c13;
	c[13] = c14;
	c[14] = c15;
	c[15] = c16;

	for (i = 0; i < 6; i++) {
		if ((parseSyntaxCondition(&co, c[i][0][0], c[i][1]) != 0) || (co->champ1->table != atoi(c[i][2])) || (co->champ1->row != atoi(c[i][3])) || (strcmp(co->comparisonOperator, c[i][4]) != 0) || (co->champ2->table != atoi(c[i][5])) || (co->champ2->row != atoi(c[i][6]))) {
			PRINT_ERROR(i + 1);
			returnValue = 1;
		}
		libererCondition((void **) &co);
	}
	for (i = 6; i < 16; i++) {
		if (parseSyntaxCondition(&co, c[i][0][0], c[i][1]) == 0) {
			PRINT_ERROR(i + 1);
			returnValue = 1;
		}
		libererCondition((void **) &co);/* par sécurité */
	}

	return returnValue;
}

int main(void) {
	printf("parseSyntaxChamp_test : \n");
	if (parseSyntaxChamp_test() == 0) {
		printf("\tOK !\n");
	}
	printf("parseSyntaxCondition_test : \n");
	if (parseSyntaxCondition_test() == 0) {
		printf("\tOK !\n");
	}

	return 0;
}
