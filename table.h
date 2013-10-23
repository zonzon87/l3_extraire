#ifndef __TABLE_H__
#define __TABLE_H__

#include "file.h"


typedef struct table {
	int row;
	file lines;
} table;

extern int createTable(table ** tab, );

extern int createTables(file * tables, requete * reqIn, requete ** reqOut);


#endif
