/* pgnmerge.c, Michael George, 14 jul 1999 */

/* Source_2 is the most correct PGN file. */

/* Voir si on peut remplacer exit(1) par return */

#define destination_path "d:\\aeffacer\\ic9899m.pgn"
#define source_1_path "d:\\aeffacer\ic9899n.pgn"
#define source_2_path "d:\\aeffacer\ic9899.pgn"
#define known_tags 26
#define comparable_tags {0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialize_tags_content(char tags_content[][])
 { register i;

   for (i=0; i<257; i++)
      tags_content[i][0]='\0';
 }
int identify_tag(char tag_string[])
 { char *tag_name[]={"[Event","[Site","[Date","[Round","[White",
       "[Black","[Result","[WhiteTitle","[BlackTitle","[WhiteElo",
       "[BlackElo","[WhiteCountry","[BlackCountry","[WhiteTeam",
       "[BlackTeam","[Board","[Division","[Section","[Stage","[Mode",
       "[Termination","[TimeControl","[ECO","[NIC","[Opening",
       "[Variation"};
   int tag_number=0;
   register i;

   for (i=0; (i<known_tags)&&(!tag_number); i++)
      if (!strcmp(tag_name[i],tag_string)) tag_number=i+1;
   return tag_number;
 }
void read_tag_section(FILE file,char word[],char tags_content[][])
 { int tag_number,comparable_tag[]=comparable_tags;

   while (strcmp(word,"1."))
     { tag_number=identify_tag(word);
       /* Is it a known tag? */
       if (!tag_number)
         { printf("Unknown tag (%s).",word);
           exit(1);
         }
       else
         { fscanf(file,"%s",word);
           /* Does this content match a potential previous one? */
           if ((tags_content[tag_number-1][1]!='\0')
              &&(comparable_tag[tag_number-1])
              &&(strcmp(word,tags_content[tag_number-1]))
             { printf("Tag contents do not match (%s<>%s).",tags_content[tag_number-1],word);
               exit(1);
             }
           else
              strcpy(tags_content[tag_number-1],word);
         }
       fscanf(file,"%s",word);
     }
 }
void ignore_move_section(FILE *file,char word[])
 { char word[257];

   while (strcmp(word,"[Event"))
      fscanf(file,"%s\n",word);
 }
void read_write_move_section(FILE *source,FILE *destination,char )
 { char line[257];

   fprintf(destination,"1.");
   fscanf(source,"%[^\n],line");
   while (line[0]!='[')
    { fprintf(destination,"%s\n",line);
      fscanf(source,"%[^\n],line");
    }
 }
void write_tag_section(FILE *destination,char tag_content[][])
 { char *tag_name[]={"[Event","[Site","[Date","[Round","[White",
       "[Black","[Result","[WhiteTitle","[BlackTitle","[WhiteElo",
       "[BlackElo","[WhiteCountry","[BlackCountry","[WhiteTeam",
       "[BlackTeam","[Board","[Division","[Section","[Stage","[Mode",
       "[Termination","[TimeControl","[ECO","[NIC","[Opening",
       "[Variation"};
   register i;

   for (i=0; i<known_tags; i++)
      if (tag_content[i][0]!='\0')
         printf("%s \"%s\"]\n",tag_name[1],tag_content[i]);
 }
int loadding(void)
 { FILE *source_1,*source_2,*destination;
   int games=0,known_result,loadding_ok,processed=0,tag_number,temp
      files_open=0;
   char word[257],tag_content[known_tags][257];

/* Files opening. */
   source_1=fopen(source_1_path,"r");
   if (source_1)
     { source_2=fopen(source_2_path,"r");
       if (source_2)
         { destination=fopen(destination_path,"w");
           if (destination)
              files_open=1;
           else
             { printf("Unable to open the destination file.");
               fclose(source_2);
               fclose(source_1);
             }
         }
       else
         { printf("Unable to open the second source file.");
           fclose(source_1);
         }
     }
   else
      printf("Unable to open the first source file.");
/* Reading/writing the files. */
   if (files_open)
     { printf("Merging files...\n");
       fscanf(source_1,"%s",word);
       while (!feof(source_1))
         { if (!strcmp(word,"[Event")
             { initialize_tags_content();
               read_tag_section();
               ignore_move_section();
               /* Reading source_2 */
               fscanf(source_2,"%s",word);
               if (feof(source_2)
                  { printf("File ended too soon, please abort.");
                    getch();
                  }
               read_tag_section(source_2);
               /* Writing destination */
               write_tag_section(destination,tag_content);
               /* Reading source_2/writing destination */
               read_write_move_section();
               /* Reading source_1 */
               read_tag_section(source_1);
             }
           else
             { printf("Wrong first tag, please abort.");
               getch();
             }
         }
/* Closing the files. */
       fclose(source_1);
       fclose(source_2);
       fclose(destination);
       printf("%d processed",processed);
       loadding_ok=1;
     }
   return loadding_ok;
 }
int main(void)
 { loadding;
   return 1;
 }