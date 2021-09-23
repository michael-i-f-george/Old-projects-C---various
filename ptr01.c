/* Pointers, Michael George, 30 apr 1999 */

#include <stdio.h>

int main(void)
 { int a=13,*p;

   p=&a;
   printf("\n&a: %x  a: %d   &p: %x  p: %x  *p: %d.",&a,a,&p,p,*p);
   return 1;
 }