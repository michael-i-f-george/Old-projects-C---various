/* pgnstat.c, Michael George, 22 mar 1999 */

/* ë=alt 235 */
/* Each game must begin (at/with) the first move, ie: no SetUp or FEN
   tag allowed. */
/* Corriger phrase du dessus. */
/* main: verifier initialisation racines (btree.c) */
/* display_results(): alignements qd nbres trop gds */
/* Average lentgh of a game. */
/* Do not forget le caractere "%", ";" */
/* Ne pas traiter la move section si [Result "*"] => creer
   identify_result_tag() */
/* Pgm repere tags pas le debut de tag section !!! => emploi else du
   switch dangereux. */
/* Presume que le PGN est correct => dangereux. */
/* Voir si on peut faire une fonction get_result_string(). */
/* Voir si je peux utiliser enum */
/* last_move_number(): malloc() de char ? */
/* last_move_number(): demi-coups => meilleures stats. */
/* Store file_name and searched_name in a "default.txt" and les proposer
   par defauts (cf. Linux). */
/* Do not work with too few games (ex: dix_parties.pgn). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#define file_name "d:\\pgn\\copie.pgn"
#define searched_name " \"George, Michaël\""

struct key_data
 { char name[3];
   int result[4];
 };
struct node
 { struct key_data key;
   struct node *left;
   struct node *right;
 };

void insert_node(struct node **current, char chaine[], int result_number, int *nodes_qty);
void free_tree(struct node **current);
void copy_tree(struct node *current,struct node **array,int *node_number);
void quick_sort(struct node **array,int left_boundary,int right_boundary);
int min(int x, int y, int z);
void display_results(struct node *White_array,struct node *Black_array,
   int White_keys_qty,int Black_keys_qty);
int move_number(char word[]);
int last_move_number(FILE *file, char word[],int result_number);
int identify_result_string(char result_string[]);
inot identify_tag(char tag_string[]);
int loadding(struct node **White_tree,struct node **Black_tree,
   int *White_keys_qty, int *Black_keys_qty);

int main(void)
 { struct node *Black_array,*White_array,*Black_tree,*White_tree;
   int Black_keys_qty=0,White_keys_qty=0,temp;

   /* Read the file and create two binary trees (one black and one
      white). */
   White_tree=Black_tree=NULL;
   if (loadding(&White_tree,&Black_tree,&White_keys_qty,&Black_keys_qty))
     { /* Convert the binarie trees into arrays. */
       White_array=(struct node *) malloc(White_keys_qty*sizeof(struct node));
       temp=White_keys_qty;
       copy_tree(White_tree,&White_array,&temp);
       free_tree(&White_tree);
       Black_array=(struct node *) malloc(Black_keys_qty*sizeof(struct node));
       temp=Black_keys_qty;
       copy_tree(Black_tree,&Black_array,&temp);
       free_tree(&Black_tree);
       /* Sort the arrays. */
       quick_sort(&White_array,0,White_keys_qty-1);
       quick_sort(&Black_array,0,Black_keys_qty-1);
       /* Display the results. */
       display_results(White_array,Black_array,White_keys_qty,Black_keys_qty);
       /* Free the memory. */
       if (White_array) free(White_array);
       if (Black_array) free(Black_array);
     }
   return 0;
 }

void insert_node(struct node **current, char chaine[], int result_number, int *nodes_qty)
 { /* Insert the node in the right binary tree (black or white). */
   struct node *new_node;

   if (*current)
     { if (strcmp((*current)->key.name,chaine)<0)
          insert_node(&((*current)->left),chaine,result_number,nodes_qty);
       else if (strcmp((*current)->key.name,chaine)>0)
               insert_node(&((*current)->right),chaine,result_number,nodes_qty);
            else
              { (*current)->key.result[result_number-1]++;
                (*current)->key.result[3]++;
              }
     }
   else
     { new_node=(struct node *) malloc(sizeof(struct node));
       strcpy(new_node->key.name,chaine);
       new_node->key.result[0]=0;
       new_node->key.result[1]=0;
       new_node->key.result[2]=0;
       new_node->key.result[3]=1;
       new_node->key.result[result_number-1]=1;
       new_node->left=NULL;
       new_node->right=NULL;
       (*nodes_qty)++;
       *current=new_node;
     }
 }
void free_tree(struct node **current)
 { if (*current)
     { free_tree(&((*current)->left));
       free_tree(&((*current)->right));
       free(*current);
     }
 }
void copy_tree(struct node *current,struct node **array,int *node_number)
 { /* Convert the binary tree into an array. */
   if (current)
     { copy_tree(current->left,array,node_number);
       (*node_number)--;
       (*array)[*node_number]=*current;
       copy_tree(current->right,array,node_number);
     }
 }
void quick_sort(struct node **array,int left_boundary,int right_boundary)
 { /* From "Le Grand Livre de la programmation en langage C",
      Gerhard Willms, Micro Application. */
   register i;
   int delimiter,pivot;
   struct node temp;

   if (left_boundary>=right_boundary) return;
   pivot=(left_boundary+right_boundary)/2;
   /* Save the pivot value. */
   temp=(*array)[left_boundary];
   (*array)[left_boundary]=(*array)[pivot];
   (*array)[pivot]=temp;
   delimiter=left_boundary;
   /* Swap the lower values to the delimiter left. */
   for (i=left_boundary+1; i<=right_boundary; i++)
      if ((*array)[i].key.result[3]>(*array)[left_boundary].key.result[3])
        { temp=(*array)[++delimiter];
          (*array)[delimiter]=(*array)[i];
          (*array)[i]=temp;
        }
   /* Restore the pivot value. */
   temp=(*array)[left_boundary];
   (*array)[left_boundary]=(*array)[delimiter];
   (*array)[delimiter]=temp;
   quick_sort(array,left_boundary,delimiter-1);
   quick_sort(array,delimiter+1,right_boundary);
 }
int min(int x, int y, int z)
 { if (y<x) x=y;
   if (z<x) x=z;
   return x;
 }
void display_results(struct node *White_array,struct node *Black_array,
   int White_keys_qty,int Black_keys_qty)
 { int W_others[]={0,0,0,0},B_others[]={0,0,0,0},boundary,
      W_total[]={0,0,0,0},B_total[]={0,0,0,0};
   float percentage;
   register i;

   printf("\n\n             White                               Black\n");
   printf("key      1-0  1/2  0-1      %       key      1-0  1/2  0-1      %");
/* Print both White & Black keys at the same time. */
   boundary=min(9,White_keys_qty,Black_keys_qty);
   for (i=0; i<boundary; i++)
     { percentage=50.0*(White_array[i].key.result[2]*2+White_array[i].key.result[1])/White_array[i].key.result[3];
       printf("\n%s     %5d%5d%5d%7.1f",White_array[i].key.name,
          White_array[i].key.result[2],White_array[i].key.result[1],
          White_array[i].key.result[0],percentage);
       percentage=50.0*(Black_array[i].key.result[0]*2+Black_array[i].key.result[1])/Black_array[i].key.result[3];
       printf("       %s     %5d%5d%5d%7.1f",Black_array[i].key.name,
          Black_array[i].key.result[2],Black_array[i].key.result[1],
          Black_array[i].key.result[0],percentage);
       W_total[0]+=White_array[i].key.result[0];
       W_total[1]+=White_array[i].key.result[1];
       W_total[2]+=White_array[i].key.result[2];
       B_total[0]+=Black_array[i].key.result[0];
       B_total[1]+=Black_array[i].key.result[1];
       B_total[2]+=Black_array[i].key.result[2];
     }
/* Print the remaining White keys to reach 9 lines. */
   boundary=min(White_keys_qty,9,9);
   for (; i<boundary; i++)
     { percentage=50.0*(White_array[i].key.result[2]*2+White_array[i].key.result[1])/White_array[i].key.result[3];
       printf("\n%s     %5d%5d%5d%7.1f",White_array[i].key.name,
          White_array[i].key.result[2],White_array[i].key.result[1],
          White_array[i].key.result[0],percentage);
       W_total[0]+=White_array[i].key.result[0];
       W_total[1]+=White_array[i].key.result[1];
       W_total[2]+=White_array[i].key.result[2];
     }
/* Print the remaining Black keys to reach 9 lines. */
   boundary=min(Black_keys_qty,9,9);
   for (; i<boundary; i++)
     { percentage=50.0*(Black_array[i].key.result[0]*2+Black_array[i].key.result[1])/Black_array[i].key.result[3];
       printf("       %s     %5d%5d%5d%7.1f",Black_array[i].key.name,
          Black_array[i].key.result[2],Black_array[i].key.result[1],
          Black_array[i].key.result[0],percentage);
       B_total[0]+=Black_array[i].key.result[0];
       B_total[1]+=Black_array[i].key.result[1];
       B_total[2]+=Black_array[i].key.result[2];
     }
/* Compute White's "Others". */
   for (i=9; i<White_keys_qty; i++)
     { W_others[0]+=White_array[i].key.result[0];
       W_others[1]+=White_array[i].key.result[1];
       W_others[2]+=White_array[i].key.result[2];
     }
   W_others[3]=W_others[0]+W_others[1]+W_others[2];
/* Compute Black's "Others". */
   for (i=9; i<Black_keys_qty; i++)
     { B_others[0]+=Black_array[i].key.result[0];
       B_others[1]+=Black_array[i].key.result[1];
       B_others[2]+=Black_array[i].key.result[2];
     }
   B_others[3]=B_others[0]+B_others[1]+B_others[2];
/* Print totals. */
   percentage=50.0*(W_others[2]*2+W_others[1])/W_others[3];
   printf("\nOthers %5d%5d%5d%7.1f",W_others[2],W_others[1],
      W_others[0],percentage);
   percentage=50.0*(B_others[0]*2+B_others[1])/B_others[3];
   printf("       Others %5d%5d%5d%7.1f",B_others[2],B_others[1],
      B_others[0],percentage);
   W_total[0]+=W_others[0];
   W_total[1]+=W_others[1];
   W_total[2]+=W_others[2];
   W_total[3]=W_total[0]+W_total[1]+W_total[2];
   B_total[0]+=B_others[0];
   B_total[1]+=B_others[1];
   B_total[2]+=B_others[2];
   B_total[3]=B_total[0]+B_total[1]+B_total[2];
   percentage=50.0*(W_total[2]*2+W_total[1])/W_total[3];
   printf("\nTotal  %5d%5d%5d%7.1f",W_total[2],W_total[1],W_total[0],
      percentage);
   percentage=50.0*(B_total[0]*2+B_total[1])/B_total[3];
   printf("       Total  %5d%5d%5d%7.1f",B_total[2],B_total[1],
      B_total[0],percentage);
   percentage=50.0*((W_total[2]+B_total[0])*2+W_total[1]+B_total[1])/(W_total[3]+B_total[3]);
   printf("\n\nScore: + %d  = %d  - %d%7.1f\%\n",W_total[2]+B_total[0],
      W_total[1]+B_total[1],W_total[0]+B_total[2],percentage);
 }
int move_number(char word[])
 { /* Return zero if not a move number. */
   int periods_qty=0,digits_qty=0,i=0,move_number=0;

   while (word[i]>='0'&&word[i]<='9')
     { digits_qty++;
       i++;
     }
   while (word[i]=='.')
     { periods_qty++;
       i++;
     }
   if (digits_qty&&(periods_qty==1||periods_qty==3)&&word[i]=='\0')
      move_number=atol(word);
   return move_number;
 }
int last_move_number(FILE *file, char word[],int result_number)
 { char *result_strings[]={" \"0-1\""," \"1/2-1/2\""," \"1-0\""," \"*\""};
   char result_string[11];
   int last_move_number=0,temp;
/* char escape_characters[]={'%',';'};
   char nesting_characters[]={'{','(',')','}'};
   int nest_qty=0; */

   strcpy(result_string,result_strings[result_number-1]);
   while (strcmp(word,result_string))
     { temp=move_number(word);
       if (temp) last_move_number=temp;
       fscanf(file,"%s",word);
     }
   return last_move_number;
 }
int identify_result_string(char result_string[])
 { /* Return zero if not a result. */
   char *possible_values[]={" \"0-1\""," \"1/2-1/2\""," \"1-0\""," \"*\""};
   int result_number=0;
   register i;

   for (i=0; (i<4)&&(!result_number); i++)
      if (!strcmp(possible_values[i],result_string)) result_number=i+1;
   return result_number;
 }
int identify_tag(char tag_string[])
 { char *possible_values[]={"[Event","[White","[Black","[Result","[NIC"};
   int tag_number=0;
   register i;

   for (i=0; (i<5)&&(!tag_number); i++)
      if (!strcmp(possible_values[i],tag_string)) tag_number=i+1;
   return tag_number;
 }
int loadding(struct node **White_tree,struct node **Black_tree,
   int *White_keys_qty, int *Black_keys_qty)
 { FILE *file;
   int color,games=0,known_player,loadding_ok,processed=0,
      result_number,tag_number,temp;
   char word[256], NIC_string[3];

   file=fopen(file_name,"r");
   if (!file)
     { printf("Unable to open the file.\n");
       loadding_ok=0;
     }
   else
     { printf("Score of %s.\n\n",searched_name);
       printf("Scanning games...\n");
       fscanf(file,"%s",word);
       while (!feof(file))
         { tag_number=identify_tag(word);
           switch (tag_number)
             { case 1: /* Event tag. */
                       known_player=result_number=0;
                       games++;
                       break;
               case 2: /* White tag. */
                       fscanf(file,"%[^]]",word);
                       if (!strcmp(word,searched_name))
                         { color=0;
                           known_player=1;
                         }
                       break;
               case 3: /* Black tag. */
                       fscanf(file,"%[^]]",word);
                       if (!strcmp(word,searched_name))
                         { color=1;
                           known_player=1;
                         }
                       break;
               case 4: /* Result tag. */
                       fscanf(file,"%[^]]",word);
                       result_number=identify_result_string(word);
                       break;
               case 5: /* NIC tag. */
                       if (known_player&&result_number&&result_number!=4)
                         { fscanf(file,"%s",word);
                           if (strlen(word)==3)
                             { /* Retrieve the opening initials. */
                               NIC_string[0]=word[1];
                               NIC_string[1]=word[2];
                               NIC_string[2]='\0';
                               /* Choose the right binary tree (black or
                                  white). */
                               if (color)
                                  insert_node(Black_tree,NIC_string,
                                     result_number,Black_keys_qty);
                               else
                                  insert_node(White_tree,NIC_string,
                                     result_number,White_keys_qty);
                               processed++;
                             }
                         }
             }
           fscanf(file,"%s",word);
         }
       fclose(file);
       printf("%d processed, %d ignored (unknown result, no NIC or/and player not found).",
           processed,games-processed);
       loadding_ok=1;
     }
   return loadding_ok;
 }