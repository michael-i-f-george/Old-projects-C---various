#include <iostream.h>
#include <conio.h>

class matrice
  {    int elmt[4][4];
    public:
       void initialisation1(void);
       void initialisation2(void);
       void affiche(void);
       matrice somme(matrice &);
       matrice operator + (matrice &);
       matrice operator - (matrice &);
  };

void matrice::initialisation1()
 { elmt[0][0]=1;
   elmt[0][1]=2;
   elmt[0][2]=3;
   elmt[0][3]=4;
   elmt[1][0]=5;
   elmt[1][1]=6;
   elmt[1][2]=7;
   elmt[1][3]=8;
   elmt[2][0]=9;
   elmt[2][1]=10;
   elmt[2][2]=11;
   elmt[2][3]=12;
   elmt[3][0]=13;
   elmt[3][1]=14;
   elmt[3][2]=15;
   elmt[3][3]=16;
 }
void matrice::initialisation2()
 { elmt[0][0]=3;
   elmt[0][1]=1;
   elmt[0][2]=0;
   elmt[0][3]=0;
   elmt[1][0]=1;
   elmt[1][1]=3;
   elmt[1][2]=1;
   elmt[1][3]=0;
   elmt[2][0]=0;
   elmt[2][1]=1;
   elmt[2][2]=3;
   elmt[2][3]=1;
   elmt[3][0]=0;
   elmt[3][1]=0;
   elmt[3][2]=1;
   elmt[3][3]=3;
 }
void matrice::affiche()
 { int i,j;

   cout << endl;
   for (i=0; i<4; i++)
     { for (j=0; j<4; j++)
          cout << " " << elmt[i][j];
       cout << endl;
     }
 }
matrice matrice::operator - (matrice & op2)
 { int i,j;
   matrice resultat;

   for (i=0; i<4; i++)
      for (j=0; j<4; j++)
         resultat.elmt[i][j]=elmt[i][j]-op2.elmt[i][j];
   return resultat;
 }
matrice matrice::somme(matrice & op2)
 { int i,j;
   matrice resultat;

   for (i=0; i<4; i++)
      for (j=0; j<4; j++)
         resultat.elmt[i][j]=elmt[i][j]+op2.elmt[i][j];
   return resultat;
 }
matrice matrice::operator + (matrice & op2)
 { int i,j;
   matrice resultat;

   for (i=0; i<4; i++)
      for (j=0; j<4; j++)
         resultat.elmt[i][j]=elmt[i][j]+op2.elmt[i][j];
   return resultat;
 }
main()
 { matrice A,B,S1,S2,D;

   clrscr();
   A.initialisation1();
   A.affiche();
   B.initialisation2();
   B.affiche();
   S1=A.somme(B);
   S1.affiche();
   S2=A+B;
   S2.affiche();
   D=A-B;
   D.affiche();
   return 1;
 }