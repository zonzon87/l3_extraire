/*
	make compilation
	gcc -ansi -Wall -Wextra -pedantic -ggdb -o outils_test outils.o outils_test.c
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./outils_test
*/
#include <stdio.h>
#include <string.h>

#include "outils.h"


int copierCharEtoile_test() {
	char c1[] = "Bonjour, comment vas-tu ?";
	void * c2;

	copierCharEtoile(c1, &c2);

	if (strcmp(c1, c2) != 0) {
		return 1;
	}
	libererSimple((void **) &c2);

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

	return 0;
}
