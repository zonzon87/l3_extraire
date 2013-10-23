#ifndef __TABLE_H__
#define __TABLE_H__

#include "file.h"
#include "requete.h"


typedef struct table {
	int row;
	file lines;
} table;


extern int createTable(table ** tab, char * fileName, int ordreLength, int * ordre);

extern int createTables(file * tables, requete * reqIn, requete ** reqOut);


#endif
