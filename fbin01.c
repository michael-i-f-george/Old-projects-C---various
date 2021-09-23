#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct date
 { int jour;
   int mois;
   int annee;
 };
struct personne
 { char prenom[20];
   struct date ddn;
 };

void ecriture(FILE **fichier);
void lecture(FILE *fichier);

int main(void)
 { FILE *fichier;

   ecriture(&fichier);
   lecture(fichier);
   return 0;
 }
void ecriture(FILE **fichier)
 { char temp_char[20];
   int i, temp_int;
   struct personne *nouveau;

   nouveau=(struct personne*) malloc (sizeof(struct personne));
   (*fichier)=fopen("d:\\aeffacer\\aeffacer.bin","wb");
   for (i=0; i<3; i++)
     { printf(">\n");
       scanf("%d",&temp_int);
       nouveau->ddn.jour=temp_int;
       scanf("%d",&temp_int);
       nouveau->ddn.mois=temp_int;
       scanf("%d",&temp_int);
       nouveau->ddn.annee=temp_int;
       scanf("%s",temp_char);
       strcpy(nouveau->prenom,temp_char);
       fwrite(&nouveau,sizeof(struct personne),4,*fichier);
     }
   fclose(*fichier);
 }
void lecture(FILE *fichier)
 { struct personne *nouveau;

   nouveau=(struct personne*) malloc (sizeof(struct personne));
   fichier=fopen("d:\\aeffacer\\aeffacer.bin","rb");
   fread(&nouveau,sizeof(struct personne),4,fichier);
   while (!feof(fichier))
     { printf("%d ",nouveau->ddn.jour);
       printf("%d ",nouveau->ddn.mois);
       printf("%d ",nouveau->ddn.annee);
       printf("%s\n",nouveau->prenom);
       fread(&nouveau,sizeof(struct personne),4,fichier);
     }
   fclose(fichier);
 }