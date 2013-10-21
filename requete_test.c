/*
	make compilation
	gcc -ansi -Wall -Wextra -pedantic -ggdb -lm -o requete_test file.o outils.o requete.o requete_test.c
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./requete_test
*/
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
		libererCondition((void **) &co); /* Par sécurité. */
	}

	return returnValue;
}

int createRequete_test() {
	int i = 1;
	int returnValue = 0;
	requete * req = NULL;

	/* Les tests suivants doivent être corrects. */
	const char * c1In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1"};
	const char * c1Out[] = {"10", "4", "2", "1", " ", NULL};
	const char * c2In[] = {"./extraire", "a.1", "b.1", "a.2", "c.2", "d.8", "b.3", "de", "sport.table", "repas.table", "jeu.table", "compte.table", "avec", "a.1=b.1"};
	const char * c2Out[] = {"14", "6", "4" ,"1", " ", NULL};
	const char * c3In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "a.3 <. b.2", "[unique]"};
	const char * c3Out[] = {"12", "4", "2", "2", "u", NULL};
	const char * c4In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "a.3 >= b.2", "ordre", "a.3"};
	const char * c4Out[] = {"13", "4", "2", "1", "o", NULL};
	const char * c5In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "a.3 >= b.2", "ordre", "a.1"};
	const char * c5Out[] = {"13", "4", "2", "1", "o", NULL};

	char ** cIn[5];
	cIn[0] = c1In;
	cIn[1] = c2In;
	cIn[2] = c3In;
	cIn[3] = c4In;
	cIn[4] = c5In;
	char ** cOut[5];

	if	(
		(createRequete(&req, 10, c1In) != 0) ||
		(file_taille(req->champsSortie) != 4) ||
		(file_taille(req->nomsTables) != 2) ||
		(file_taille(req->conditions) != 1) ||
		(req->option != ' ') ||
		(req->champOrdre != NULL)
	) {
		PRINT_ERROR(i);
		returnValue = 1;
	}
	destroyRequete(&req);


	

	/*for (i = 0; i < 6; i++) {
		if (createRequete_test(&req, )) {
			PRINT_ERROR(i + 1);
			returnValue = 1;
		}
	}
	for (i = 6; i < 16; i++) {
		if () {
			PRINT_ERROR(i + 1);
			returnValue = 1;
		}
	}*/

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
	printf("createRequete_test : \n");
	if (createRequete_test() == 0) {
		printf("\tOK !\n");
	}

	return 0;
}
