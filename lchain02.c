/* Liste chainee, Michael George, 25 mars 1999. */

#include <stdio.h>    /* printf(), scanf() */
#include <conio.h>    /* clrscr(), getch() */
#include <alloc.h>    /* malloc() */
#include <string.h>   /* strcpy() */

typedef struct element
  { char nom[3];
    int parties=0;
    struct element *suivant;
  };

struct element* cree_liste(void)
 { char chaine[3];
   struct element *debut=NULL, *nouveau;

   scanf("%s",chaine);
	while (strcmp(chaine,"zz"))
	  { nouveau=(struct element *) malloc(sizeof(struct element));
       strcpy(nouveau->nom,chaine);
       nouveau->suivant=debut;
       debut=nouveau;
       scanf("%s",chaine);
     }
   return debut;
 }
struct element* cree_liste(struct element *courant)
 { char chaine[3];
   int trouve=0;

   scanf("%s",chaine);
   while (courant&&(!trouve))
     { if (!strcmp(courant->nom,chaine)) courant->parties++;
       courant=courant->suivant;
     }
 }
void affiche_liste(struct element *courant)
 { while (courant)
	  { printf("\n%s",courant->nom);
       courant=courant->suivant;
	  }
 }
void trie_liste(struct element *debut)
 { struct element *courant, *deja_trie=NULL;
   char temp[3];

   if (debut)
     { while (debut->suivant!=deja_trie)
         { courant=debut;
           while (courant->suivant!=deja_trie)
             { if (strcmp(courant->nom,courant->suivant->nom)<0)
                 { strcpy(temp,courant->nom);
                   strcpy(courant->nom,courant->suivant->nom);
                   strcpy(courant->suivant->nom,temp);
                 }
               courant=courant->suivant;
             }
           deja_trie=courant;
         }
     }
 }
int main(void)
 { struct element *debut;

   clrscr();
   debut=cree_liste();
   trie_liste(debut);
   affiche_liste(debut);
   getch();
   return 1;
 }
