#include <stdio.h>

int main(int argc, char * argv[]) {
	int i = 0;
	for (i = 0; i < argc; i++) {
		printf("Nombre d'arguments : %d.\n", argc);
		printf("%s\n", argv[i]);
	}
    return 0;
}
