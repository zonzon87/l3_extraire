/*****************************************************************************
 *                                                                           *
 *   Copyright © 2013: Alexis Lavie, Hugo Thibaud.                           *
 *                                                                           *
 *   This file is licensed under the GPLv3: General Public License v3.       *
 *                                                                           *
 *****************************************************************************/

#include <stdlib.h>

#include "file.h"
#include "file_extend.h"


#define FILE_ERREUR_NOT_A_FILE_PARCOURS -1


int file_creer(file * file_lieu, void (* copier)(const void * valeur, void ** lieu), void (* liberer)(void ** lieu)) {
	(* file_lieu) = (file) malloc(sizeof (file_struct));
	(* file_lieu)->head = NULL;
	(* file_lieu)->size = 0;
	(* file_lieu)->copier = copier;
	(* file_lieu)->liberer = liberer;

	return 0;
}

int file_detruire(file * list) {
	void * olddata = NULL;

	if ((* list) == NULL) {
		return FILE_ERREUR_NOT_A_FILE;
	}
	/* On retire tous les éléments de la file. */
	while (file_est_vide(* list) != FILE_ERREUR_FILE_VIDE) {
		file_retirer(* list, &olddata);
		(* ((* list)->liberer))(&olddata);
	}
	/* On détruit la file. */
	free(* list);
	(* list) = NULL;

	return 0;
}

int file_ajouter(file list, const void * valeur) {
	maillon m = NULL;

	if (list == NULL) {
		return FILE_ERREUR_NOT_A_FILE;
	}
	m = (maillon) malloc(sizeof (maillon_struct));

	(* (list->copier))(valeur, &(m->data));

	if (file_est_vide(list) == FILE_ERREUR_FILE_VIDE) {
		m->next = NULL;
		list->head = m;
		list->tail = m;
	} else {
		m->next = NULL;
		list->tail->next = m;
		list->tail = m;
	}
	(list->size)++;

	return 0;
}

int file_retirer(file list, void ** lieu) {
	maillon m = NULL;

	if (list == NULL) {
		return FILE_ERREUR_NOT_A_FILE;
	}
	if (file_est_vide(list) != FILE_ERREUR_FILE_VIDE) {
		(* (list->copier))(list->head->data, lieu);
		(* (list->liberer))(&(list->head->data));

		/* Si l'on retire l'unique maillon de la file. */
		if (list->head->next == NULL) {
			free(list->head);
			list->head = NULL;
		} else {
			m = list->head->next;
			free(list->head);
			list->head = m;
		}
		(list->size)--;
	}

	return 0;
}

int file_est_vide(file list) {
	if (list == NULL) {
		return FILE_ERREUR_NOT_A_FILE;
	}
	if (list->head == NULL) {
		return FILE_ERREUR_FILE_VIDE;
	}

	return 0;
}

int file_taille(file list) {
	if (list == NULL) {
		return FILE_ERREUR_NOT_A_FILE;
	}

	return list->size;
}

file_parcours file_parcours_creer(file list) {
	file_parcours parcours = NULL;

	if (list != NULL) {
		parcours = (file_parcours) malloc(sizeof (file_parcours_struct));
		parcours->list = list;
		parcours->pointerMaillon = parcours->list->head;
	}

	return parcours;
}

void file_parcours_detruire(file_parcours * parcours) {
	free(* parcours);
	(* parcours) = NULL;
}

void file_parcours_suivant(file_parcours parcours, void ** lieu) {
    (* (parcours->list->copier))(parcours->pointerMaillon->data, lieu);
    parcours->pointerMaillon = parcours->pointerMaillon->next;
}

void file_parcours_suivant_lien(file_parcours parcours, void ** lieu) {
    (* lieu) = parcours->pointerMaillon->data;
    parcours->pointerMaillon = parcours->pointerMaillon->next;
}


int file_parcours_est_fini(file_parcours parcours) {
	if (parcours == NULL) {
		return FILE_ERREUR_NOT_A_FILE_PARCOURS;
	}
	if (parcours->pointerMaillon == NULL) {
		return 1;
	}

	return 0;
}

void file_parcours_reset(file_parcours parcours) {
	parcours->pointerMaillon = parcours->list->head;
}

void libererFile_parcours(void ** lieu) {
	file_parcours_detruire((file_parcours *) lieu);
}
