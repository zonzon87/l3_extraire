#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "requete.h"

int main(int argc, char * argv[]) {
	int i = 0;
	printf("Nombre d'arguments : %d\n", argc);
	for (i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}

    return 0;
}
