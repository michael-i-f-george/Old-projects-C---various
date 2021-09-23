/* Arbre binaire (preparation seconde session), Michael George, 25 aout
   1999. */

#include <stdio.h>    /* printf(), scanf() */
#include <stdlib.h>   /* free(), malloc() */
#include <string.h>   /* strcmp(), strcpy() */

typedef struct arbre
 { char mot[10];
   struct arbre *gauche;
   struct arbre *droite;
 } ARBRE;

void affiche_trie(ARBRE *courant);
void insere_noeud(ARBRE **courant,char nouveau_mot[10]);
void supprime_arbre(ARBRE **courant);
void affiche_par_niveau(ARBRE *courant,int niveau);

int main(void)
 { ARBRE *debut;
   char temp[10];
   int i;

   debut=NULL;
   for (i=0; i<8; i++)
     { scanf("%s",temp);
       insere_noeud(&debut,temp);
     }
   printf("---\n");
   affiche_trie(debut);
   printf("---\n");
   affiche_par_niveau(debut,3);
   supprime_arbre(&debut);
   return 0;
 }

void insere_noeud(ARBRE **courant,char nouveau_mot[])
 { ARBRE *nouveau;

   if (*courant)
      if (strcmp(nouveau_mot,(*courant)->mot)<0)
         insere_noeud(&((*courant)->gauche),nouveau_mot);
      else
         insere_noeud(&((*courant)->droite),nouveau_mot);
   else
     { nouveau=(ARBRE*) malloc(sizeof(ARBRE));
       strcpy(nouveau->mot,nouveau_mot);
       nouveau->gauche=nouveau->droite=NULL;
       *courant=nouveau;
     }
 }
void affiche_trie(ARBRE *courant)
 { if (courant)
     { affiche_trie(courant->gauche);
       printf("%s\n",courant->mot);
       affiche_trie(courant->droite);
     }
 }
void affiche_par_niveau(ARBRE *courant,int niveau)
 { if (courant)
     { niveau--;
       affiche_par_niveau(courant->gauche,niveau);
       if (!niveau) printf("%s ",courant->mot);
       affiche_par_niveau(courant->droite,niveau);
       niveau++;
     }
 }
void supprime_arbre(ARBRE **courant)
 { if (*courant)
     { supprime_arbre(&((*courant)->gauche));
       supprime_arbre(&((*courant)->droite));
       free(*courant);
     }
 }