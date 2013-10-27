/*****************************************************************************
 *                                                                           *
 *   Copyright © 2013: Alexis Lavie, Hugo Thibaud.                           *
 *                                                                           *
 *   This file is licensed under the GPLv3: General Public License v3.       *
 *                                                                           *
 *****************************************************************************/

#ifndef __TABLE_H__
#define __TABLE_H__

#include "file.h"
#include "file_extend.h"
#include "xearray.h"

#define TOKENDELIMITOR "|"
#define TOKENGARBAGE ' '
#define ERROR_INEXISTANTCHAMP 2
#define P_ERROR_INEXISTANTCHAMP(x, y) printf("Erreur demande d'une colonne inexistante d'un fichier : %s >%d", x, y)
#define ERROR_MALFORMEDFILE 3
#define P_ERROR_MALFORMEDFILE(x) printf("Fichier malformé : %s", x)


typedef xEArray tableLine;

typedef file_struct table;

typedef xEArray tables;


extern void destroyTable(void ** tab);

extern void destroyTables(tables ** tEA);

extern int countNumberOfChamps(char * str, const char * delimitor, int * nbTokenAtFirstLine);

extern int divideCharEToCharEArray(tableLine ** dest, int nbElements, int nbTokenAtFirstLine, const char * delimitor, char * src);

extern int rearrangeLineRows(tableLine ** cEAOut, tableLine * cEAIn, const file_parcours values, int nbValues);

extern int createTable(table ** tab, const char * fileName, const file ordreApparitions);

extern int createTables(tables ** tEA, const file nomsTables, const file * ordreApparitions);

extern void tableLineToPrint(tableLine * cEA);

extern void tableToPrint(table * tab);


#endif
