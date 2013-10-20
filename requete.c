#include <stdio.h> /* à enlever */
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "requete.h"
#include "file.h"
#include "outils.h"


void copierChamp(const void * valeur, void ** lieu) {
	champ * ch = (champ *) valeur;
	(* lieu) = (champ *) malloc(sizeof (champ));
	((champ *) (* lieu))->table = ch->table;
	((champ *) (* lieu))->row = ch->row;
}

void copierCondition(const void * valeur, void ** lieu) {
	condition * co = (condition *) valeur;
	(* lieu) = (condition *) malloc(sizeof (condition));

	copierChamp((void *) (co->champ1), (void **) &(((condition *) (* lieu))->champ1));
	copierCharEtoile((void *) (co->comparisonOperator), (void **) &(((condition *) (* lieu))->comparisonOperator));
	copierChamp((void *) (co->champ2), (void **) &(((condition *) (* lieu))->champ2));
}

void libererCondition(void ** lieu) {
	if ((* lieu) != NULL) {
		libererSimple((void **) &(((condition *) (* lieu))->champ1));
		libererSimple((void **) &(((condition *) (* lieu))->comparisonOperator));
		libererSimple((void **) &(((condition *) (* lieu))->champ2));
		libererSimple(lieu);
	}
}

void destroyRequete(requete ** req) {
	file_detruire(&((* req)->champsSortie));
	file_detruire(&((* req)->nomsTables));
	file_detruire(&((* req)->conditions));
	libererSimple((void **) req);
}

int base26to10(int * result, const char * str, const int strLength) {
	int i;
	int temp;

	for (i = 0; i < strLength; i++) {
		temp = (int) str[i];
		if ((temp > 96) && (temp < 123)) { /* Si le caractère est compris entre 'a' et 'z' inclus. */
			(* result) += (temp - 96) * pow(26, strLength - i - 1);
		} else {
			return 1;
		}
	}
	(* result)--;

	return 0;
}

int parseSyntaxChamp(champ ** ch, const char separatorChamp, const char * c) {
	int a;
	int length;
	int separatorChampPosition = -1;

	int tableNumber;
	int rowNumber;

	length = (int) strlen(c);
	for (a = 0; a < length; a++) {
		if (c[a] == separatorChamp) {
			if (separatorChampPosition < 0) {
				separatorChampPosition = a;
			} else {
				return SYNTAX_CHAMP_EXCEPTION;
			}
		}
	}

	if (!((separatorChampPosition > 0) && (separatorChampPosition < (length - 1)))) {
		return SYNTAX_CHAMP_EXCEPTION;
	}

	{
		int tokenLength;
		char * token = NULL;

		tableNumber = 0;
		rowNumber = 0;

		/* To comment */
		tokenLength = separatorChampPosition;
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, c, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		a = base26to10(&tableNumber, token, tokenLength);
		libererSimple((void **) &token);

		if (a != 0) {
			return SYNTAX_CHAMP_EXCEPTION;
		}

		/* To comment */
		tokenLength = length - (separatorChampPosition + 1);
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, c + separatorChampPosition + 1, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		a = isNumeric(token, tokenLength);
		rowNumber = atoi(token);
		libererSimple((void **) &token);

		if ((a != 0) || (rowNumber < 1)) {
			return SYNTAX_CHAMP_EXCEPTION;
		} else {
			rowNumber--;
		}
	}
	
	(* ch) = (champ *) malloc(sizeof (champ));
	(* ch)->table = tableNumber;
	(* ch)->row = rowNumber;

	return 0;
}

int parseSyntaxCondition(condition ** co, const char separatorChamp, const char * c) {
	int a;
	int length;
	int separatorConditionPositionStart = -1;
	int separatorConditionPositionEnd = -1;

	champ * champ1 = NULL;
	char * comparisonOperator = NULL;
	champ * champ2 = NULL;

	length = (int) strlen(c);
	{
		char lastChar = '\0';
		int cOTempI = 0;
		char cOTemp[] = {'\0', '\0', '\0', '\0'};

		for (a = 0; a < length; a++) {
			if (!(separatorConditionPositionStart < 0)) {
				if	(
					(((lastChar == '<') || (lastChar == '>')) && (isInVAList(c[a], 2, '=', '.') == 0))
					||
					((lastChar == '!') && (c[a] == '='))
					||
					((lastChar == '=') && (c[a] == '.'))
					)
				{
					cOTemp[cOTempI] = c[a];
					cOTempI++;
				} else {
					separatorConditionPositionEnd = a - 1;
					break;
				}
			} else {
				if (isInVAList(c[a], 4, '<', '>', '!', '=') == 0) {
					cOTemp[cOTempI] = c[a];
					cOTempI++;
					separatorConditionPositionStart = a;
				}
			}
			lastChar = c[a];
		}
		if (!(((separatorConditionPositionStart > 0) && (separatorConditionPositionEnd > 0)) &&
			((separatorConditionPositionStart < (length - 1)) && (separatorConditionPositionEnd < (length - 1))))) {
			return SYNTAX_CONDITION_EXCEPTION;
		}
		comparisonOperator = (char *) malloc((sizeof (char)) * (cOTempI + 1));
		memcpy(comparisonOperator, cOTemp, (sizeof (char)) * cOTempI);
		comparisonOperator[cOTempI] = '\0';
	}

	{
		int tokenLength;
		char * token = NULL;

		/* To comment */
		tokenLength = separatorConditionPositionStart;
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, c, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		a = parseSyntaxChamp(&champ1, separatorChamp, token);
		libererSimple((void **) &token);

		if (a != 0) {
			libererSimple((void **) &comparisonOperator);
			libererSimple((void **) &champ1);
			return a;
		}

		tokenLength = length - (separatorConditionPositionEnd + 1);
		token = (char *) malloc((sizeof (char)) * (tokenLength + 1));
		memcpy(token, c + separatorConditionPositionEnd + 1, (sizeof (char)) * tokenLength);
		token[tokenLength] = '\0';

		a = parseSyntaxChamp(&champ2, separatorChamp, token);
		libererSimple((void **) &token);

		if (a != 0) {
			libererSimple((void **) &comparisonOperator);
			libererSimple((void **) &champ1);
			libererSimple((void **) &champ2);
			return a;
		}
	}

	(* co) = (condition *) malloc(sizeof (condition));
	(* co)->champ1 = champ1;
	(* co)->comparisonOperator = comparisonOperator;
	(* co)->champ2 = champ2;

	return 0;
}

requete * analyzeArgs(const int argc, const char * argv[]) {
	/* rappel: argv[0] = #commande d'appel# */
	const char of[] = "de";
	const char with[] = "avec";
	const char separatorChamp='.';

	int i;
	int ofPosition = -1;
	int withPosition = -1;
	requete * req = NULL;
	int catched = 0;

	req = (requete *) malloc(sizeof (requete));
	file_creer(&(req->champsSortie), &copierChamp, &libererSimple);
	file_creer(&(req->nomsTables), &copierCharEtoile, &libererSimple);
	file_creer(&(req->conditions), &copierCondition, &libererCondition);

	TRY {
		/* Test du nombre d'arguments. */
		if (argc < 7) { /* 6 paramètres au minimum. */
			THROW(MISSING_ARGS_EXCEPTION);
		}

		/* Vérification des mots-clés. */
		{
			while (i < argc) {
				if (strcmp(argv[i], of) == 0) { /* On cherche un unique mot clé "de". */
					if (ofPosition < 0) {
						ofPosition = i;
					} else {
						THROW(KEYWORD_EXCEPTION);
					}
				}
				if (strcmp(argv[i], with) == 0) { /* On cherche un unique mot clé "avec". */
					if (withPosition < 0) {
						withPosition = i;
					} else {
						THROW(KEYWORD_EXCEPTION);
					}
				}
				i++;
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
		}

		/* Vérification syntaxique et ajout des champs à extraire. */
		{
			int pSCResult;
			champ * tempChamp = NULL;

			for (i = 1; i < ofPosition; i++) {
				pSCResult = parseSyntaxChamp(&tempChamp, separatorChamp, argv[i]);
				if (pSCResult == 0) {
					file_ajouter(req->champsSortie, tempChamp);
				} else {
					libererSimple((void **) &tempChamp);
					THROW(pSCResult);
				}
			}

			libererSimple((void **) &tempChamp);
		}

		/* Vérification syntaxique et ajout des conditions d'extraction. */
		{
			int pSCResult;
			condition * tempCondition = NULL;

			for (i = withPosition + 1; i < argc; i++) {
				pSCResult = parseSyntaxCondition(&tempCondition, separatorChamp, argv[i]);
				if (pSCResult == 0) {
					file_ajouter(req->conditions, tempCondition);
				} else {
					libererCondition((void **) &tempCondition);
					THROW(pSCResult);
				}
			}
		}

		/* Enregistrement des chemins des fichiers. */
		{
			for (i = ofPosition + 1; i < withPosition; i++) {
				file_ajouter(req->nomsTables, argv[i]);
			}
		}

	} CATCH (MISSING_ARGS_EXCEPTION) {
		catched = 1;
	} CATCH (KEYWORD_EXCEPTION) {
		catched = 1;
	} CATCH (SYNTAX_CHAMP_EXCEPTION) {
		catched = 1;
	} CATCH (SYNTAX_CONDITION_EXCEPTION) {
		catched = 1;
	} CATCH (CONDITION_EXCEPTION) {
		catched = 1;
	} CATCH (INCOHERENT_REQUETE_EXCEPTION) {
		catched = 1;
	} FINALLY {
		/* On clean tout si une exception a été levée. */
		if (catched) {
			destroyRequete(&req);
		}
	} ETRY;

	return NULL;
}
