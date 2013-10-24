#ifndef __TABLE_H__
#define __TABLE_H__

#include "file.h"
#include "outils.h"
#include "requete.h"

#define TOKENDELIMITOR "|"
#define TOKENGARBAGE ' '
#define ERROR_MISSINGFILE 1
#define P_ERROR_MISSINGFILE(x) printf("Fichier inexistant : %s\n", x)
#define ERROR_INEXISTANTCHAMP 2
#define P_ERROR_INEXISTANTCHAMP(x, y) printf("Erreur demande d'une colonne inexistante d'un fichier : %s.%d\n", x, y)
#define ERROR_MALFORMEDFILE 3
#define P_ERROR_MALFORMEDFILE(x) printf("Fichier malformé : %s\n", x)


typedef struct table {
	int nbLines;
	file lines;
} table;


extern void destroyTable(table ** tab);

extern int countNumberOfChamps(const char * ch, const char * delimitor);

extern int divideCharEtoiletoCharEtoileArray(charEtoileArray ** dest, int nbElements, const char * delimitor, char * src);

extern int rearrangeLineRows(charEtoileArray ** cEAOut, charEtoileArray * cEAIn, file_parcours values, int nbValues);

extern int createTable(table ** tab, const char * fileName, file ordreApparitions, int maxValue);

extern int createTables(file * tables, requete * reqIn, requete ** reqOut);


#endif
