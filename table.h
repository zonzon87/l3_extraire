#ifndef __TABLE_H__
#define __TABLE_H__

#include "file.h"
#include "requete.h"

#define TABLE_BUFFER 128
#define TOKENDELIMITOR "|"
#define LINE_EOF 1
#define ERROR_MISSINGFILE 1
#define P_ERROR_MISSINGFILE(x) printf("Fichier inexistant : %s\n", x)
#define ERROR_INEXISTANTCHAMP 2
#define P_ERROR_INEXISTANTCHAMP(x, y) printf("Erreur demande d'une colonne inexistante d'un fichier : %s.%d\n", x, y)


typedef struct ordreApparition {
	int maxValue;
	int nbValues;
	int * values;
} ordreApparition;


typedef struct table {
	int row;
	file lines;
} table;


extern void destroyTable(table ** tab);

/*	Permet la copie d'une ligne d'un fichier, en supposant que le curseur de
	fichier est positionné 1 caractère avant le premier de la ligne à extraire.
	Entrée : char ** line avec (* line) non-alloué, FILE * fichier ouvert
		(non NULL).
	Sortie : char * via (* line) de taille TABLE_BUFFER * n (où est le
		minimum pour stocker la chaîne).
	- supporte \r (Mac OS) \r\n (Windows) \n (UNIX) comme marqueurs de fin de
		ligne.
	- la chaîne retournée est dépourvu du marqueur de fin de ligne lu.
	- marqueur de fin de chaîne '\0' présent.
	- garantit que (* line) soit différent de NULL.
*/
extern int getLine(char ** line, FILE * fichier);

extern int countNumberOfChamps(char * ch);

extern int createTable(table ** tab, char * fileName, ordreApparition * ord);

extern int createTables(file * tables, requete * reqIn, requete ** reqOut);


#endif
