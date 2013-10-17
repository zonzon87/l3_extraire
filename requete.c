#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
#define SYNTAX_CHAMP_EXCEPTION 3
#define CONDITION_EXCEPTION 4

void copierChamp(const void * valeur, void ** lieu) {
	champ * in = (champ *) valeur;
	(* lieu) = (champ *) malloc(sizeof (champ));
	((champ *)(* lieu))->table = in->table;
	((champ *)(* lieu))->row = in->row;
}

void copierCharEtoile(const void * valeur, void ** lieu) {
	(* lieu) = (char *) malloc(sizeof ((char *) valeur));
	memcpy((* lieu), valeur, sizeof ((char *) valeur));
}

void liberer(void ** lieu) {
	free(* lieu);
	(* lieu) = NULL;
}

int parseSyntaxChamp(const char * separatorChamp, const char * c, int * fileNumber, int * rowNumber) {
	int j;
	int length;
	int separatorChampPosition = -1;

	length = (int) strlen(c);
	for (j = 0; j < length; j++) {
		if (strcmp(c, separatorChamp) == 0) {
			if (separatorChampPosition < 0) {
				separatorChampPosition = j;
			} else {
				return SYNTAX_CHAMP_EXCEPTION;
			}
		}
	}
	if (!((separatorChampPosition > 0) && (separatorChampPosition < (length - 1)))) {
		return SYNTAX_CHAMP_EXCEPTION;
	}

	{
		int temp;
		int tokenLength;
		char * token;

		(* fileNumber) = 0;
		(* rowNumber) = 0;
		tokenLength = separatorChampPosition;
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, c, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		for (j = 0; j < tokenLength; j++) {
			temp = (int) token[j];
			if ((temp >= 97) && (temp <= 122)) { /* Si le caractère est compris entre 'a' et 'z' inclus. */
				(* fileNumber) += (temp - 96) * pow(26, (tokenLength - j - 1));
			} else {
				return SYNTAX_CHAMP_EXCEPTION;
			}
		}

		(* fileNumber)--;

		free(token);

		tokenLength = length - (separatorChampPosition + 1);
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, c, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		(* rowNumber) = atoi(token);

		free(token);
	}

	return 0;
}

requete * analyzeArgs(int argc, const char * argv[]) {
	/* rappel: argv[0] = #commande d'appel# */
	const char of[] = "de";
	const char with[] = "avec";
	const char separatorChamp='.';

	int argCount = 0;
	int ofPosition = -1;
	int withPosition = -1;
	requete * req = NULL;

	req = (requete *) malloc(sizeof (requete));
	file_creer(&(req->champsSortie), &copierChamp, &liberer);
	file_creer(&(req->nomsTables), &copierCharEtoile, &liberer);

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
				(withPosition < argc - 1)) /* Au minimum une condition. */
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
			int fileNumber;
			int rowNumber;
			int pSCResult;
			champ * tempChamp = NULL;

			tempChamp = (champ *) malloc(sizeof (champ));

			for (i = 1; i < ofPosition; i++) {
				pSCResult = parseSyntaxChamp(&separatorChamp, argv[i], &fileNumber, &rowNumber);
				if (pSCResult == 0) {
					tempChamp->table = fileNumber;
					tempChamp->row = rowNumber;
					file_ajouter(req->champsSortie, tempChamp);
				} else {
					THROW(pSCResult);
				}
			}

			free(tempChamp);
		}

		/* Enregistrement des chemins des fichiers */
		{
			int i;
			for (i = ofPosition + 1; i < withPosition; i++) {
				file_ajouter(req->nomsTables, argv[i]);
			}
		}

	} CATCH (MISSING_ARGS_EXCEPTION) {

	} CATCH (KEYWORD_EXCEPTION) {

	} CATCH (SYNTAX_CHAMP_EXCEPTION) {

	} CATCH (CONDITION_EXCEPTION) {

	} FINALLY {
		/* On clean tout si une exception a été levée. */
	} ETRY;

	/*	file_detruire();
		free(req);
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

int destroyRequete(requete ** req) {
	return 0;
}