#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "requete.h"
#include "outils.h"


int parseSyntaxChamp_test() {
	int returnValue = 0;
	int i;
	
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
		PRINT_T(i + 1);
		if	(
			(parseSyntaxChamp(&ch, c[i][0][0], c[i][1]) != 0) ||
			(ch->table != atoi(c[i][2])) ||
			(ch->row != atoi(c[i][3]))
		) {
			PRINT_T_ERROR();
			returnValue = 1;
		} else {
			PRINT_T_OK();
		}
		libererSimple((void **) &ch);
	}
	for (i = 4; i < 12; i++) {
		PRINT_T(i + 1);
		if (parseSyntaxChamp(&ch, c[i][0][0], c[i][1]) == 0) {
			PRINT_T_ERROR();
			returnValue = 1;
		} else {
			PRINT_T_OK();
		}
	}

	return returnValue;
}

int parseSyntaxCondition_test() {
	int returnValue = 0;
	int i;
	
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
		PRINT_T(i + 1);
		if	(
			(parseSyntaxCondition(&co, c[i][0][0], c[i][1]) != 0) ||
			(co->champ1->table != atoi(c[i][2])) ||
			(co->champ1->row != atoi(c[i][3])) ||
			(strcmp(co->comparisonOperator, c[i][4]) != 0) ||
			(co->champ2->table != atoi(c[i][5])) ||
			(co->champ2->row != atoi(c[i][6]))
		) {
			PRINT_T_ERROR();
			returnValue = 1;
		} else {
			PRINT_T_OK();
		}
		libererCondition((void **) &co);
	}
	for (i = 6; i < 16; i++) {
		PRINT_T(i + 1);
		if (parseSyntaxCondition(&co, c[i][0][0], c[i][1]) == 0) {
			PRINT_T_ERROR();
			returnValue = 1;
		} else {
			PRINT_T_OK();
		}
	}

	return returnValue;
}

int initRequete_test() {
	int returnValue = 0;
	int i;

	requete * req = NULL;

	/* Les tests suivants doivent être corrects. */
	const char * c1In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1"};
	const char * c1Out[] = {"10", "4", "2", "1", " "};
	const char * c2In[] = {"./extraire", "a.1", "b.1", "a.2", "c.2", "d.8", "b.3", "de", "sport.table", "repas.table", "jeu.table", "compte.table", "avec", "a.1=b.1"};
	const char * c2Out[] = {"14", "6", "4" ,"1", " "};
	const char * c3In[] = {"./extraire", "a.1", "b.1", "a.1", "a.2", "b.3", "de", "sport.table", "repas.table", "jeu.table", "compte.table", "avec", "a.1=c.1", "a.3<.d.2", "[unique]"};
	const char * c3Out[] = {"15", "5", "4", "2", "u"};
	const char * c4In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "a.3>=b.2", "ordre", "b.1"};
	const char * c4Out[] = {"13", "4", "2", "2", "o"};
	const char * c5In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "a.3>=b.2", "ordre", "a.1"};
	const char * c5Out[] = {"13", "4", "2", "2", "o"};

	/* Les tests suivants doivent être incorrects. */
	const char * c6In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec"};
	const char * c6Out[] = {"9"};
	const char * c7In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "avec", "a.1=b.1"};
	const char * c7Out[] = {"9"};
	const char * c8In[] = {"./extraire", "de", "sport.table", "de", "repas.table", "avec", "a.1=b.1"};
	const char * c8Out[] = {"7"};
	const char * c9In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "c.1=b.1"};
	const char * c9Out[] = {"10"};
	const char * c10In[] = {"./extraire", "c.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1"};
	const char * c10Out[] = {"10"};
	const char * c11In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "a.3>=b.2", "ordre", "a.3"};
	const char * c11Out[] = {"13"};
	const char * c12In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "[unique]", "ordre", "a.2"};
	const char * c12Out[] = {"13"};
	const char * c13In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "ordre", "a.1", "b.1"};
	const char * c13Out[] = {"13"};
	const char * c14In[] = {"./extraire", "a.1", "grand.1", "a.2", "tropgrand.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1"};
	const char * c14Out[] = {"10"};

	const char ** cIn[14];
	const char ** cOut[14];
	cIn[0] = c1In;
	cOut[0] = c1Out;
	cIn[1] = c2In;
	cOut[1] = c2Out;
	cIn[2] = c3In;
	cOut[2] = c3Out;
	cIn[3] = c4In;
	cOut[3] = c4Out;
	cIn[4] = c5In;
	cOut[4] = c5Out;
	cIn[5] = c6In;
	cOut[5] = c6Out;
	cIn[6] = c7In;
	cOut[6] = c7Out;
	cIn[7] = c8In;
	cOut[7] = c8Out;
	cIn[8] = c9In;
	cOut[8] = c9Out;
	cIn[9] = c10In;
	cOut[9] = c10Out;
	cIn[10] = c11In;
	cOut[10] = c11Out;
	cIn[11] = c12In;
	cOut[11] = c12Out;
	cIn[12] = c13In;
	cOut[12] = c13Out;
	cIn[13] = c14In;
	cOut[13] = c14Out;

	for (i = 0; i < 5; i++) {
		PRINT_T(i + 1);
		if	(
			(initRequete(&req, atoi(cOut[i][0]), cIn[i]) != 0) ||
			(file_taille(req->champsSortie) != atoi(cOut[i][1])) ||
			(file_taille(req->nomsTables) != atoi(cOut[i][2])) ||
			(file_taille(req->conditions) != atoi(cOut[i][3])) ||
			(req->option != cOut[i][4][0]) ||
			((req->option == C_ORDER) && (req->champOrdre == NULL))
		) {
			PRINT_T_ERROR();
			returnValue = 1;
		} else {
			PRINT_T_OK();
		}
		destroyRequete(&req);
	}
	for (i = 5; i < 14; i++) {
		PRINT_T(i + 1);
		if (initRequete(&req, atoi(cOut[i][0]), cIn[i]) == 0) {
			PRINT_T_ERROR();
			returnValue = 1;
		} else {
			PRINT_T_OK();
		}
	}

	return returnValue;
}

void createRequete_test() {
	int returnValue = 0;
    int i;

	requete * req = NULL;

	/* Les tests suivants sont corrects. */
	const char * c1In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1"};
	const char * c1Out[] = {"10"};
	const char * c2In[] = {"./extraire", "a.1", "b.1", "a.2", "c.2", "d.8", "b.3", "de", "sport.table", "repas.table", "jeu.table", "compte.table", "avec", "a.1=b.1"};
	const char * c2Out[] = {"14"};
	const char * c3In[] = {"./extraire", "a.1", "b.1", "a.1", "a.2", "b.3", "de", "sport.table", "repas.table", "jeu.table", "compte.table", "avec", "a.1=c.1", "a.3<.d.2", "[unique]"};
	const char * c3Out[] = {"15"};
	const char * c4In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "a.3>=b.2", "ordre", "b.1"};
	const char * c4Out[] = {"13"};
	const char * c5In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "a.3>=b.2", "ordre", "a.1"};
	const char * c5Out[] = {"13"};

    /* Les tests suivants doivent être incorrects. */
	const char * c6In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec"};
	const char * c6Out[] = {"9"};
	const char * c7In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "avec", "a.1=b.1"};
	const char * c7Out[] = {"9"};
	const char * c8In[] = {"./extraire", "de", "sport.table", "de", "repas.table", "avec", "a.1=b.1"};
	const char * c8Out[] = {"7"};
	const char * c9In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "c.1=b.1"};
	const char * c9Out[] = {"10"};
	const char * c10In[] = {"./extraire", "c.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1"};
	const char * c10Out[] = {"10"};
	const char * c11In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "a.3>=b.2", "ordre", "a.3"};
	const char * c11Out[] = {"13"};
	const char * c12In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "[unique]", "ordre", "a.2"};
	const char * c12Out[] = {"13"};
	const char * c13In[] = {"./extraire", "a.1", "b.1", "a.2", "b.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1", "ordre", "a.1", "b.1"};
	const char * c13Out[] = {"13"};
	const char * c14In[] = {"./extraire", "a.1", "grand.1", "a.2", "tropgrand.3", "de", "sport.table", "repas.table", "avec", "a.1=b.1"};
	const char * c14Out[] = {"10"};

    const char ** cIn[14];
	const char ** cOut[14];
	cIn[0] = c1In;
	cOut[0] = c1Out;
	cIn[1] = c2In;
	cOut[1] = c2Out;
	cIn[2] = c3In;
	cOut[2] = c3Out;
	cIn[3] = c4In;
	cOut[3] = c4Out;
	cIn[4] = c5In;
	cOut[4] = c5Out;
    cIn[5] = c6In;
	cOut[5] = c6Out;
	cIn[6] = c7In;
	cOut[6] = c7Out;
	cIn[7] = c8In;
	cOut[7] = c8Out;
	cIn[8] = c9In;
	cOut[8] = c9Out;
	cIn[9] = c10In;
	cOut[9] = c10Out;
	cIn[10] = c11In;
	cOut[10] = c11Out;
	cIn[11] = c12In;
	cOut[11] = c12Out;
	cIn[12] = c13In;
	cOut[12] = c13Out;
	cIn[13] = c14In;
	cOut[13] = c14Out;

    for (i = 0; i < 5; i++) {
		createRequete(&req, atoi(cOut[i][0]), cIn[i]);
		destroyRequete(&req);
	}
	printf("### Poubelle ###\n");
	for (i = 5; i < 14; i++) {
		createRequete(&req, atoi(cOut[i][0]), cIn[i]);
	}
	printf("\n### Poubelle ###\n");
}

int main(void) {
	printf("parseSyntaxChamp_test() : \n");
	if (parseSyntaxChamp_test() == 0) {
		PRINT_OK();
	}
	printf("parseSyntaxCondition_test() : \n");
	if (parseSyntaxCondition_test() == 0) {
		PRINT_OK();
	}
	printf("initRequete_test() : \n");
	if (initRequete_test() == 0) {
		PRINT_OK();
	}
	printf("createRequete_test() : Memory only\n");
    createRequete_test();

	return 0;
}
