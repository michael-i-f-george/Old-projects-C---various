/* Tri rapide, Michael George, 11 dec 1998 */

#include <stdio.h>   /* printf() */

void tri_rapide(int nbre[],int borne_inferieure,int borne_superieure);

int main (void)
 { int nbre[]={6,11,5,4,12,14,12,17,12,9,13,12,18,16,7,19,21};
	register i;

   printf("\n");
	for (i=0; i<16; i++)
      printf(" %d",nbre[i]);
	tri_rapide(nbre,0,16);
   printf("\n");
	for (i=0; i<16; i++)
      printf(" %d",nbre[i]);
   return 0;
 }
void tri_rapide(int nbre[],int borne_inferieure,int borne_superieure)
 { register i;
   int temp,separateur,pivot;

   if (borne_inferieure>=borne_superieure) return;
	pivot=(borne_inferieure+borne_superieure)/2;
	temp=nbre[borne_inferieure];
	nbre[borne_inferieure]=nbre[pivot];
	nbre[pivot]=temp;
	separateur=borne_inferieure;
	for (i=borne_inferieure+1; i<=borne_superieure; i++)
		if (nbre[i]<nbre[borne_inferieure])
		  { temp=nbre[++separateur];
		    nbre[separateur]=nbre[i];
		    nbre[i]=temp;
		  }
	temp=nbre[borne_inferieure];
	nbre[borne_inferieure]=nbre[separateur];
	nbre[separateur]=temp;
	tri_rapide(nbre,borne_inferieure,separateur-1);
	tri_rapide(nbre,separateur+1,borne_superieure);
 }