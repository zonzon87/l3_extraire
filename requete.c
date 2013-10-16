#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

#include "requete.h"
#include "file.h"

#define TRY do { jmp_buf ex_buf; switch(setjmp(ex_buf)){ case 0: while(1) {
#define CATCH(x) break; case x:
#define FINALLY break; } default:
#define ETRY } } while(0)
#define THROW(x) longjmp(ex_buf, x)

#define MISSING_ARGS_EXCEPTION 1
#define KEYWORD_EXCEPTION 2
#define EXTRACTED_CHAMP_EXCEPTION 3
#define CONDITION_EXCEPTION 4

void copierChamp(const void * valeur, void ** lieu) {
	champ * in = (champ *) valeur;
	(* lieu) = (champ *) malloc(sizeof (champ));
	((champ *)(* lieu))->c = in->c;
	((champ *)(* lieu))->c = in->n;
}

void copierCharEtoile(const void * valeur, void ** lieu) {
	(* lieu) = (char *) malloc(sizeof ((char *) valeur));
	memcpy((* lieu), valeur, sizeof ((char *) valeur));
}

void liberer(void ** lieu) {
	free(* lieu);
	(* lieu) = NULL;
}

parameters * analyzeArgs(int argc, char * argv[]) {
	/* rappel: argv[0] = #commande d'appel# */
	const char of[] = "de";
	const char with[] = "avec";

	int argCount = 0;
	int ofPosition = -1;
	int withPosition = -1;
	char separatorChamp='.';
	parameters * param = NULL;
	
	champ temp;

	param = (parameters *) malloc(sizeof (parameters));
	file_creer(&(param->champsSortie), &copierChamp, &liberer);

	TRY {
		/* Test du nombre d'arguments. */
		if (argc < 7) { /* 6 paramètres au minimum. */
			THROW(MISSING_ARGS_EXCEPTION);
		}

		/* Vérification des mots-clés. */
		argCount = 1;
		while (argCount < argc) {
			if (strcmp(argv[argCount], of) == 0) { /* On cherche un unique mot clé "de". */
				if (ofPosition < 0) {
					ofPosition = argCount;
				} else {
					THROW(KEYWORD_EXCEPTION);
				}
			}
			if (strcmp(argv[argCount], with) == 0) { /* On cherche un unique mot clé "avec". */
				if (withPosition < 0) {
					withPosition = argCount;
				} else {
					THROW(KEYWORD_EXCEPTION);
				}
			}
			argCount++;
		}
		if 	( /* Test final */
			!(
				((ofPosition > 1) && /* Au minimum un champ à extraire. */
				(withPosition < argc)) /* Au minimum une condition. */
			&&
				(withPosition - ofPosition >= 2) /* Au minimum deux fichiers entre les mots-clés. */
			)
			)
		{
			THROW(KEYWORD_EXCEPTION);
		} /* else { Tout va bien } */


		/* Vérification et ajout des champs. */
		{
			int i;
			for (i = 1; i < ofPosition; i++) {

			}
		}

		/* Enregistrement des chemins des fichiers */
		{
			int i;
			for (i = ofPosition + 1; i < withPosition; i++) {

			}
		}


		for (argCount=1; argv[argCount][1]=='.' && (argv[argCount][0]=='a' || argv[argCount][0]=='b'); ++argCount ) {
			temp.c=argv[argCount][0];
			strtok(argv[argCount], &separatorChamp);
			temp.n=atoi(strtok(NULL, &separatorChamp));
			file_ajouter(param->champsSortie, &temp);
		}

	} CATCH (MISSING_ARGS_EXCEPTION) {

	} FINALLY {

	} ETRY;

	if (strcmp(argv[argCount], "de")!=0) {
		printf("echec comparaison : de en %d\n", argCount);
		exit(0);
	}

	if (strcmp(argv[argCount+3], "avec")!=0) { 
		printf("echec comparaison : avec en %d\n", argCount+3);
		exit(0);
	}

	/*	file_detruire();
		free(param);
	*/
	

	/*
	comparaison=strtok(argv[argCount+4], egal);

	comparaison1=comparaison;
	table1=strtok(comparaison1, separatorChamp);
	comparaison1=strtok(NULL, separatorChamp);
	chercher_noeud (ma_jointure, table1[0], atoi(strtok(comparaison1, separatorChamp)));	

	comparaison=strtok(NULL, egal);

	comparaison2=comparaison;
	table2=strtok(comparaison2, separatorChamp);
	comparaison2=strtok(NULL, separatorChamp);
	chercher_noeud (ma_jointure, table2[0], atoi(strtok(comparaison2, separatorChamp)));
	*/

	return NULL;

}

int destroyRequete(parameters ** param) {
	return 0;
}