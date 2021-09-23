/* Remanence des fonctions recursives, Michael George, 18 apr 1999 */

#include <stdio.h> /* printf() */

void recursive(int a)
 { printf(" %d",a++);
   if (a<10) recursive(a);
 }
int main(void)
 { printf("\n");
   recursive(0);
   return 0;
 }