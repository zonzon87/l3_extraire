# Les modules
MODULE := file xearray requete table outils
# Fichiers en-tête
HEADERS := $(MODULE:%=%.h)

# Compilateur
CC :=  gcc
# Options de compilation
CFLAGS := -ansi -Wall -Wextra -pedantic -ggdb
# Bibliothèques
LFLAGS := -lm
# Options de valgrind
VFLAGS := --leak-check=full --show-reachable=yes --track-origins=yes

.PHONY : all compilation test memoire alltest file_test xearray_test outils_test requete_test table_test archive clean cleanexec

# Construction par défaut :
all :
	@echo "Disponible à la construction :"
	@echo "compilation => compilation extraire.c"
	@echo "test => test simple sur une requete"
	@echo "memoire => teste les fuites mémoire"
	@echo "alltest => lance tout les tests de module existants"
	@echo "file_test => petit test du module file avec un test mémoire"
	@echo "xearray_test => test sur le module xearray, avec valgrind"
	@echo "outils_test => test sur le module outils, avec valgrind"
	@echo "requete_test => test sur le module requete, avec valgrind"
	@echo "table_test => test sur le module table, avec valgrind"
	@echo "archive => produit la fichier pour le rendu"
	@echo "clean => nettoie le répertoire *.o"
	@echo "cleanexec => nettoie le répertoire des exécutables"


# Compilation
compilation : ./extraire 

# Uniquement pour requete.o
requete.o : requete.c requete.h
	$(CC) -c $(CFLAGS) $(LFLAGS) -o $@ $<

# Règle de compilation
%.o : %.c $(HEADERS) 
	$(CC) -c $(CFLAGS) -o $@ $<

# Compilation de l'exécutable extraire
./extraire : $(MODULE:%=%.o) $(HEADERS) extraire.c
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $(MODULE:%=%.o) extraire.c

# Répertoire de données pour les tests
DONNES_DIR := Donnees
# Répertoire où mettre les résultats
RESULTATS_DIR := Resultats
# Répertoire où sont les résultats attendus
RESULTATS_ATTENDUS_DIR := Resultats_Attendus

# Requêtes pour les tests sur extraire
TEST_REQUETE_1 := a.1 b.1 a.2 b.3 de $(DONNES_DIR)/sport.table $(DONNES_DIR)/repas.table avec a.1=b.1
TEST_REQUETE_2 := a.3 b.5 a.4 b.2 b.9 de $(DONNES_DIR)/r2-1.table $(DONNES_DIR)/r2-2.table avec a.3=b.5

# FAIRE un TEST SIMPLE
test : extraire
	@mkdir -p $(RESULTATS_DIR)
	@./extraire $(TEST_REQUETE_1) 1> $(RESULTATS_DIR)/requete_1.table
	@if ! diff $(RESULTATS_DIR)/requete_1.table $(RESULTATS_ATTENDUS_DIR)/requete_1.table ; then echo "*** RÉSUTALT INCORRECT ***" ; false ; else echo "Test 1 OK !" ; fi
	@./extraire $(TEST_REQUETE_2) 1> $(RESULTATS_DIR)/requete_2.table
	@if ! diff $(RESULTATS_DIR)/requete_2.table $(RESULTATS_ATTENDUS_DIR)/requete_2.table ; then echo "*** RÉSUTALT INCORRECT ***" ; false ; else echo "Test 2 OK !" ; fi

# TEST de GESTION de la MÉMOIRE
memoire : extraire
	@valgrind $(VFLAGS) ./extraire $(TEST_REQUETE_1) >/dev/null
	@valgrind $(VFLAGS) ./extraire $(TEST_REQUETE_2) >/dev/null

# Lance tout les tests de module existants
alltest : file_test xearray_test outils_test requete_test table_test

# TEST du module FILE avec un TEST MÉMOIRE
file_test : file.o file_test.c
	@mkdir -p $(RESULTATS_DIR)
	@$(CC) $(CFLAGS) -o $@ file.o file_test.c
	@valgrind $(VFLAGS) ./$@ > $(RESULTATS_DIR)/file_test.output
	@if ! diff $(RESULTATS_DIR)/file_test.output $(RESULTATS_ATTENDUS_DIR)/file_test.output ; then echo "*** RÉSUTALT INCORRECT ***" ; false ; else echo "OK !" ; fi

# TEST du module EARRAY avec un TEST MÉMOIRE
xearray_test : xearray.o outils.o xearray_test.c
	@$(CC) $(CFLAGS) -o $@ $^
	@valgrind $(VFLAGS) ./$@

# TEST du module OUTILS avec un TEST MÉMOIRE
outils_test : outils.o outils_test.c
	@$(CC) $(CFLAGS) -o $@ $^
	@valgrind $(VFLAGS) ./$@

# TEST du module REQUETE avec un TEST MÉMOIRE
requete_test : requete.o file.o outils.o requete_test.c
	@$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^
	@valgrind $(VFLAGS) ./$@

# TEST du module TABLE avec un TEST MÉMOIRE
table_test : table.o file.o xearray.o outils.o requete.o table_test.c
	@$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^
	@valgrind $(VFLAGS) ./$@

# PRODUIT l'ARCHIVE à RENDRE par MAIL
archive :
	@tar czf PASD_mini-projet.tgz Makefile *.c *.h LICENSE README compte-rendu.pdf

clean :
	rm -v *.o

cleanexec :
	rm -v extraire file_test outils_test requete_test table_test xearray_test
