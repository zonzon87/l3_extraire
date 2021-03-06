/*****************************************************************************
 *                                                                           *
 *   Copyright © 2013: Alexis Lavie, Hugo Thibaud.                           *
 *                                                                           *
 *   This file is licensed under the GPLv3: General Public License v3.       *
 *                                                                           *
 *****************************************************************************/

#ifndef __FILE_EXTEND_H__
#define __FILE_EXTEND_H__


typedef struct maillon_struct {
	void * data;
	struct maillon_struct * next;
} maillon_struct;

typedef struct maillon_struct * maillon;

typedef struct file_struct {
	maillon head;
	maillon tail;
	int size;
	void (* copier)(const void * valeur, void ** lieu);
	void (* liberer)(void ** lieu);
} file_struct;

typedef struct file_parcours_struct {
	file list;
	maillon pointerMaillon;
} file_parcours_struct;

extern void file_parcours_suivant_lien(file_parcours parcours, void ** lieu);

extern void file_parcours_reset(file_parcours parcours);

extern void libererFile_parcours(void ** lieu);

#endif
