/* Liste chainee (preparation seconde session), Michael George, 25 aug
   1999. */

#include <stdio.h>    /* printf(), scanf() */
#include <stdlib.h>   /* free(), malloc() */

typedef struct NOEUD
 { char mot[10];
   struct NOEUD *suivant;
 } noeud;

void ajoute_element(noeud **courant);
void affiche_liste(noeud *courant);
void supprime_liste(noeud **courant);
void insere_element(noeud **debut);

int main(void)
 { noeud *debut;
   int i;

   debut=NULL;
   for (i=0; i<10; i++)
      ajoute_element(&debut);
   affiche_liste(debut);
   supprime_liste(&debut);
   return 0;
 }

void insere_element(noeud **debut)
 { /* Place l'element au debut de la liste. */
   noeud *nouveau;
   char temp[10];

   nouveau=(noeud*) malloc(sizeof(noeud));
   scanf("%s",nouveau->mot);
   nouveau->suivant=*debut;
   *debut=nouveau;
 }
void ajoute_element(noeud **courant)
 { /* Place l'element a la fin de la liste. */
   noeud *nouveau;
   char temp[10];

   if (*courant!=NULL)
      ajoute_element(&((*courant)->suivant));
   else
     { nouveau=(noeud*) malloc(sizeof(noeud));
       scanf("%s",nouveau->mot);
       nouveau->suivant=NULL;
       *courant=nouveau;
     }
 }
void affiche_liste(noeud *courant)
 { if (courant)
     { printf("%s\n",courant->mot);
       affiche_liste(courant->suivant);
     }
 }
void supprime_liste(noeud **courant)
 { if (*courant)
     { supprime_liste(&((*courant)->suivant));
       free(*courant);
     }
 }