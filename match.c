/* match.c, John Kercheval, jan 5 1991 */
/* Wildcard Pattern Matching */

#include "match.h"

int matche_after_star(register char *pattern, register char *text);
int fast_match_after_star(register char *pattern, register char *text);

int is_pattern (char *p)
 { /* Return 1 if PATTERN has any special wildcard characters */
   while (*p)
     { switch (*p++)
         { case '?':
           case '*':
           case '[':
           case '\\':
           return 1;
         }
     }
   return 0;
 }

/*----------------------------------------------------------------------------
*
* Return 1 if PATTERN has is a well formed regular expression according
* to the above syntax
*
* error_type is a return code based on the type of pattern error.  Zero is
* returned in error_type if the pattern is a valid one.  error_type return
* values are as follows:
*
*   PATTERN_VALID - pattern is well formed
*   PATTERN_ESC   - pattern has invalid escape ('\' at end of pattern)
*   PATTERN_RANGE - [..] construct has a no end range in a '-' pair (ie [a-])
*   PATTERN_CLOSE - [..] construct has no end bracket (ie [abc-g )
*   PATTERN_EMPTY - [..] construct is empty (ie [])
*
----------------------------------------------------------------------------*/

int is_valid_pattern (char *p, int *error_type)
 { /* init error_type */
   *error_type=PATTERN_VALID;
   /* loop through pattern to EOS */
   while(*p)
     { /* determine pattern type */
       switch(*p)
         { /* check literal escape, it cannot be at end of pattern */
           case '\\': if (!*++p)
                        { *error_type = PATTERN_ESC;
                          return 0;
                        }
                      p++;
                      break;

            /* the [..] construct must be well formed */
            case '[': p++;
                      /* if the next character is ']' then bad pattern */
                      if (*p==']')
                        { *error_type = PATTERN_EMPTY;
                          return 0;
                        }
                      /* if end of pattern here then bad pattern */
                      if (!*p)
                        { *error_type=PATTERN_CLOSE;
                          return 0;
                        }
                      /* loop to end of [..] construct */
                      while (*p!=']')
                        { /* check for literal escape */
                          if (*p=='\\')
                            { p++;
                              /* if end of pattern here then bad pattern */
                              if (!*p++)
                                { *error_type=PATTERN_ESC;
                                  return 0;
                                }
                            }
                          else 
                             p++;
                          /* if end of pattern here then bad pattern */
                          if (!*p)
                            { *error_type=PATTERN_CLOSE;
                              return 0;
                            }
                          /* if this a range */
                          if (*p=='-')
                            { /* we must have an end of range */
                              if (!*++p||*p==']')
                                { *error_type=PATTERN_RANGE;
                                  return 0;
                                }
                              else
                                { /* check for literal escape */
                                  if (*p=='\\') p++;
                                  /* if end of pattern here then bad pattern */
                                  if (!*p++)
                                    { *error_type=PATTERN_ESC;
                                      return 0;
                                    }
                                }
                            }
                        }
                      break;
            /* all other characters are valid pattern elements */
            case '*':
            case '?':
            default: p++;                              /* "normal" character */
                     break;
         }
     }

     return 1;
}


/*----------------------------------------------------------------------------
*
*  Match the pattern PATTERN against the string TEXT;
*
*  returns MATCH_VALID if pattern matches, or an errorcode as follows
*  otherwise:
*
*            MATCH_PATTERN  - bad pattern
*            MATCH_LITERAL  - match failure on literal mismatch
*            MATCH_RANGE    - match failure on [..] construct
*            MATCH_ABORT    - premature end of text string
*            MATCH_END      - premature end of pattern string
*            MATCH_VALID    - valid match
*
*
*  A match means the entire string TEXT is used up in matching.
*
*  In the pattern string:
*       `*' matches any sequence of characters (zero or more)
*       `?' matches any character
*       [SET] matches any character in the specified set,
*       [!SET] or [^SET] matches any character not in the specified set.
*
*  A set is composed of characters or ranges; a range looks like
*  character hyphen character (as in 0-9 or A-Z).  [0-9a-zA-Z_] is the
*  minimal set of characters allowed in the [..] pattern construct.
*  Other characters are allowed (ie. 8 bit characters) if your system
*  will support them.
*
*  To suppress the special syntactic significance of any of `[]*?!^-\',
*  and match the character exactly, precede it with a `\'.
*
----------------------------------------------------------------------------*/

int matche(register char *p, register char *t)
 { register char range_start, range_end;  /* start and end in range */

   int invert;             /* is this [..] or [!..] */
   int member_match;       /* have I matched the [..] construct? */
   int loop;               /* should I terminate? */

   for (; *p; p++,t++) {

        /* if this is the end of the text then this is the end of the match */
        if (!*t) {
            return ( *p == '*' && *++p == '\0' ) ? MATCH_VALID : MATCH_ABORT;
        }

        /* determine and react to pattern type */
        switch (*p) {

            /* single any character match */
            case '?':
                break;

            /* multiple any character match */
            case '*': return matche_after_star (p, t);
            /* [..] construct, single member/exclusion character match */
            case '[': { /* move to beginning of range */
                        p++;
                        /* check if this is a member match or exclusion match */
                        invert = 0;
                        if (*p=='!'||*p=='^')
                          { invert=1;
                            p++;
                          }
                        /* if closing bracket here or at range start then we have a
                           malformed pattern */
                        if (*p==']')
                          { return MATCH_PATTERN;
                          }
                        member_match=0;
                        loop=1;

                while (loop) {

                    /* if end of construct then loop is done */
                    if (*p == ']') {
                        loop = 0;
                        continue;
                    }

                    /* matching a '!', '^', '-', '\' or a ']' */
                    if (*p=='\\')
                      { range_start=range_end=*++p;
                      }
                    else
                      { range_start=range_end=*p;
                      }

                    /* if end of pattern then bad pattern (Missing ']') */
                    if (!*p)
                        return MATCH_PATTERN;

                    /* check for range bar */
                    if (*++p=='-')
                      { /* get the range end */
                        range_end=*++p;
                        /* if end of pattern or construct then bad pattern */
                        if (range_end=='\0'||range_end==']')
                            return MATCH_PATTERN;
                        /* special character range end */
                        if (range_end == '\\') {
                            range_end = *++p;

                            /* if end of text then we have a bad pattern */
                            if (!range_end)
                                return MATCH_PATTERN;
                        }

                        /* move just beyond this range */
                        p++;
                    }

                    /* if the text character is in range then match found.
                       make sure the range letters have the proper
                       relationship to one another before comparison */
                    if (range_start<range_end)
                      { if (*t >= range_start && *t <= range_end)
                          { member_match = 1;
                            loop = 0;
                          }
                      }
                    else {
                        if (*t >= range_end && *t <= range_start) {
                            member_match = 1;
                            loop = 0;
                        }
                    }
                }

                /* if there was a match in an exclusion set then no match */
                /* if there was no match in a member set then no match */
                if ((invert && member_match) ||
                   !(invert || member_match))
                    return MATCH_RANGE;

                /* if this is not an exclusion then skip the rest of the [...]
                    construct that already matched. */
                if (member_match)
                  { while (*p!=']')
                      { /* bad pattern (Missing ']') */
                        if (!*p) return MATCH_PATTERN;
                        /* skip exact match */
                        if (*p=='\\')
                          { p++;
                            /* if end of text then we have a bad pattern */
                            if (!*p) return MATCH_PATTERN;
                          }
                        /* move to next pattern char */
                        p++;
                      }
                  }
                break;
            }

            /* next character is quoted and must match exactly */
            case '\\':

                /* move pattern pointer to quoted char and fall through */
                p++;

                /* if end of text then we have a bad pattern */
                if (!*p) return MATCH_PATTERN;
            /* must match this character exactly */
            default: if (*p!=*t) return MATCH_LITERAL;
        }
    }

    /* if end of text not reached then the pattern fails */
    if (*t)
        return MATCH_END;
    else
        return MATCH_VALID;
}




int matche_after_star (register char *p, register char *t)
 { /* recursively call matche() with final segment of PATTERN and of TEXT. */
   register int match=0;
   register nextp;

    /* pass over existing ? and * in pattern */
    while (*p=='?'||*p=='*')
      { /* take one char for each ? and + */
        if (*p=='?')
          { /* if end of text then no match */
            if (!*t++)
              { return MATCH_ABORT;
              }
          }
        /* move to next char in pattern */
        p++;
      }
    /* if end of pattern we have matched regardless of text left */
    if (!*p)
      { return MATCH_VALID;
      }
    /* get the next character to match which must be a literal or '[' */
    nextp=*p;
    if (nextp=='\\')
      { nextp=p[1];
        /* if end of text then we have a bad pattern */
        if (!nextp) return MATCH_PATTERN;
      }
    /* Continue until we run out of text or definite result seen */
    do
      { /* a precondition for matching is that the next character
           in the pattern match the next character in the text or that
           the next pattern char is the beginning of a range.  Increment
           text pointer as we go here */
        if (nextp==*t||nextp=='[')
          { match = matche(p, t);
          }
        /* if the end of text is reached then no match */
        if (!*t++) match=MATCH_ABORT;
     } while ( match != MATCH_VALID && 
              match != MATCH_ABORT &&
              match != MATCH_PATTERN);
    /* return result */
    return match;
}
/* match() is a shell to matche() to return only int values. */
int match(char *p, char *t)
 { int error_type;

   error_type=matche(p,t);
   return ( error_type == MATCH_VALID ) ? 1 : 0;
 }


#ifdef TEST

   /* This test main expects as first arg the pattern and as second arg
      the match string.  Output is yaeh or nay on match.  If nay on
      match then the error code is parsed and written. */

    #include <stdio.h>

    int main(int argc, char *argv[])
    { int error, is_valid_error;

        if (argc!=3)
          { printf("Usage:  MATCH Pattern Text\n");
          }
        else
          { printf("Pattern: %s\n", argv[1]);
            printf("Text   : %s\n", argv[2]);
            if (!is_pattern(argv[1]))
              { printf("    First Argument Is Not A Pattern\n");
              }
            else
              { error=matche(argv[1],argv[2]);
                is_valid_pattern(argv[1],&is_valid_error);
                switch (error)
                  { case MATCH_VALID: printf("    Match Successful");
                                      if (is_valid_error!=PATTERN_VALID)
                                         printf(" -- is_valid_pattern() is complaining\n");
                                      else
                                         printf("\n");
                                      break;
                    case MATCH_LITERAL: printf("    Match Failed on Literal\n");
                                        break;
                    case MATCH_RANGE: printf("    Match Failed on [..]\n");
                                      break;
                    case MATCH_ABORT: printf("    Match Failed on Early Text Termination\n");
                                      break;
                    case MATCH_END: printf("    Match Failed on Early Pattern Termination\n");
                                    break;
                    case MATCH_PATTERN: switch ( is_valid_error ) {
                            case PATTERN_VALID:
                                printf("    Internal Disagreement On Pattern\n");
                                break;
                            case PATTERN_ESC:
                                printf("    Literal Escape at End of Pattern\n");
                                break;
                            case PATTERN_RANGE:
                                printf("    No End of Range in [..] Construct\n");
                                break;
                            case PATTERN_CLOSE:
                                printf("    [..] Construct is Open\n");
                                break;
                            case PATTERN_EMPTY:
                                printf("    [..] Construct is Empty\n");
                                break;
                            default:
                                printf("    Internal Error in is_valid_pattern()\n");
                        }
                        break;
                    default: printf("    Internal Error in matche()\n");
                             break;
                }
            }

        }
        return(0);
    }

#endif
