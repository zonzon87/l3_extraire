#ifndef __TABLE_H__
#define __TABLE_H__

#include "file.h"


typedef struct table {
	int row;
	file lines;
}

extern int createFileTables(file * tables, requete * req);

#endif
