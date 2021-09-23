/* Pointeur sur pointeur, Michael George, 11 dec 1998 */

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <conio.h>
#define nbr_var 3
#define nbr_cont 2

void reservation_memoire(int **nbre)
 { int i;

   if ((nbre=(int**) malloc(nbr_var*nbr_cont*sizeof(int*)))==NULL)
      printf ("prbm d\'allocation de m‚moire.");
   for (i=0; i<nbr_var; i++)
      if ((nbre[i]=(int*) malloc(nbr_cont*sizeof(int*)))==NULL)
   printf ("prbm d\'allocation de m‚moire.");
 }
int main(void)
 { int **nbre,i,j;

   clrscr();
   printf("\nVeuillez entrer 6 chiffres svp.");
	reservation_memoire(nbre);
	if ((nbre=(int**) malloc(nbr_var*nbr_cont*sizeof(int*)))==NULL)
	  printf ("\nprbm d\'allocation de m‚moire.");
	for (i=0; i<nbr_var; i++)
	   if ((nbre[i]=(int*) malloc(nbr_cont*sizeof(int*)))==NULL)
		  printf ("\nprbm d\'allocation de m‚moire.");
	for (i=0; i<nbr_var; i++)
	   for (j=0; j<nbr_cont; j++)
		  scanf("%d",&nbre[i][j]);
	for (i=0; i<nbr_var; i++)
	   for (j=0; j<nbr_cont; j++)
		  printf("\n%d",nbre[i][j]);
	getch();
	return 0;
  }