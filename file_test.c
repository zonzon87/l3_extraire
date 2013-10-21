# include <stdlib.h>   /* malloc */
# include <string.h>   /* memcpy */
# include <stdio.h>   /* printf */

# include "file.h"

typedef struct file_test_struct {
  float ff ;
  int ii ;
} file_test_struct ;


void file_test_copier  ( const void * valeur , void ** lieu ) {
  file_test_struct * f_c = ( file_test_struct * ) malloc ( sizeof ( file_test_struct ) ) ;
  memcpy ( f_c , valeur , sizeof ( file_test_struct ) );
  * lieu = ( void * ) f_c ;
}


void file_test_liberer  ( void ** lieu ) {
  free ( * lieu ) ;
  * lieu = NULL ;
}

/* nombre de valeurs pour tester la file */
# define VAL_MAX 1000 

/* Nombre de parcours pour le test */
# define NB_PARCOURS 50


/* TODO */
void test_avec_parcours () {
  file f ;
  /* Creation */
  file_creer ( & f ,
	       & file_test_copier , 
	       & file_test_liberer )  ;
  /* ajout */  
  {
    file_test_struct s ;
    int i ;
    for ( i = 0 ; i <= VAL_MAX ; i ++ ) {
      s.ii = i ;
      s.ff = i ;
      file_ajouter ( f , ( void *) & s ) ;
    }
  }

  /* Parcours */
  {
    int i ;
    for ( i = 0 ; i < NB_PARCOURS ; i ++ ) {
      /* création du parcours */
      file_parcours parcours = file_parcours_creer ( f ) ;
      /* parcours */
      while ( ! file_parcours_est_fini ( parcours ) ) {
	file_test_struct  *pt_s ;
	file_parcours_suivant ( parcours , (void **) & pt_s ) ;
	printf ( "%3i %f\n" , pt_s -> ii , pt_s -> ff ) ;
	free ( pt_s ) ;
      }
      /* Destruction du parcours */
      file_parcours_detruire ( & parcours ) ;
    }
  }
  /* Destruction de la file */
  file_detruire ( & f ) ;
}


int main ( void ) {
  file f ;
  /* Creation */
  file_creer ( & f ,
	       & file_test_copier , 
	       & file_test_liberer )  ;
  /* ajout */  
  {
    file_test_struct s ;
    int i ;
    for ( i = 0 ; i <= VAL_MAX ; i ++ ) {
      s.ii = i ;
      s.ff = i ;
      file_ajouter ( f , ( void *) & s ) ;
    }
  }

  /* vidage */
  {
    file_test_struct  *pt_s ;
    while ( ! file_est_vide ( f ) ) {
      file_retirer ( f , (void *) & pt_s );
      printf ( "%3i %f\n" , pt_s -> ii , pt_s -> ff ) ;
      free ( pt_s ) ;
    }
  }
  /* destruction */
  file_detruire ( & f ) ;
  test_avec_parcours () ;
  return 0 ;
}
