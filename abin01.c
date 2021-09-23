/* Arbre binaire, Michael George, 27 mars 1999. */

#include <stdio.h>    /* printf(), scanf() */
#include <conio.h>    /* clrscr(), getch() */
#include <alloc.h>    /* malloc() */
#include <string.h>   /* strcpy(), strcmp() */

typedef struct noeud
 { char nom[10];
   int compteur;
   struct noeud *gauche;
   struct noeud *droite;
 };

void insere_noeud(struct noeud **courant,char chaine[])
 { struct noeud *nouveau;

   if (*courant)
     { if (strcmp((*courant)->nom,chaine)<0)
          insere_noeud(&((*courant)->gauche),chaine);
       else if (strcmp((*courant)->nom,chaine)>0)
               insere_noeud(&((*courant)->droite),chaine);
            else (*courant)->compteur++;
     }
   else
     { nouveau=(struct noeud *) malloc(sizeof(struct noeud));
       strcpy(nouveau->nom,chaine);
       nouveau->compteur=1;
       nouveau->gauche=NULL;
       nouveau->droite=NULL;
       *courant=nouveau;
     }
 }
struct noeud* cree_arbre(void)
 { struct noeud *racine=NULL;
   char chaine[10];

   scanf("%s",chaine);
   while (strcmp(chaine,"fin"))
     { insere_noeud(&racine,chaine);
       scanf("%s",chaine);
     }
   return racine;
 }
void affiche_arbre(struct noeud *courant)
 { if (courant)
     { affiche_arbre(courant->gauche);
       printf("\n>%s< %d",courant->nom,courant->compteur);
       affiche_arbre(courant->droite);
	  }
 }
void desalloue_arbre(struct noeud **courant)
 { if (*courant)
     { desalloue_arbre(&((*courant)->gauche));
       desalloue_arbre(&((*courant)->droite));
       free(*courant);
	  }
 }
int main(void)
 { struct noeud *racine;

   clrscr();
   racine=cree_arbre();
   affiche_arbre(racine);
   getch();
   desalloue_arbre(&racine);
   return 0;
 }