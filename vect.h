// "Exercices en langage C++", Claude Delannoy, page 148.
// Declaration de la class vect en vue d'une utilisation par ex_9_7.cpp.

class vect
 { protected:
      int nelem;
      int *adr;
   public:
      vect(int);
      ~vect();
      int& operator[](int);
 };