/* This program compute neighbor of suffix */
/* Suffix's Neighbor: 
      position of other suffix of the same documents,
      whose position on suffix array is close to each other */

/* $1: document_id                                (doc_id)*/
/* $2: position_in the corpus                 (suffix_pos)*/
/* $3: suffix sequence number                  (suffix_id)*/
/* $4: length of longest common prefix               (lcp)*/
/* $5: the first_upper neighbor                (neighbor1)*/
/* $6: the second upper neighbor               (neighbor2)*/
/* $7: the third upper neibghor                (neighbor3)*/
/* $8: contents at the position     (text)     */
/* Note: $8 is used to understand classes, not used to compute classs or cdf */
/* -1 means, the value is not valid */

/* please Note, there is a way to computer neighbor without using
   document que. Just sort suffix file according to [FirstKey doc_id, SecondKey_suffix_id]
   Then we can compute the neighbor by looking at neighbor of file */
/* After the, we can sort the suffix file again */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXDOC 10240
static char line[10240];
static char text[10240];
static int doc_que[MAXDOC][3];
static int doc_id = -1;
static int suffix_pos = -1;
static int suffix_id = -1;
static int lcp = -1;
static int neighbor1 = -1;
static int neighbor2 = -1;
static int neighbor3 = -1;

int main()
{ /* char * p; int n; */
  int i; int j; int dummy;
 
  for(i=0;i<MAXDOC;i++) {
   for(j=0;j<3;j++) {
     doc_que[i][j] = -1;
   }
  }
  suffix_id = 0;
  while(fgets(line, sizeof(line), stdin)) {
    sscanf(line, "%d %d %d %d %d %d %d %s", 
	   &doc_id, 
	   &suffix_pos, 
	   &dummy,
	   &lcp,
	   &neighbor1,
	   &neighbor2,
	   &neighbor3,
	   text);
    if(doc_id >= MAXDOC) {
      fprintf(stderr, "too many documents, modfiy MAXDOC\n");
      exit(1);
    }
    neighbor1 = doc_que[doc_id][0];
    neighbor2 = doc_que[doc_id][1];
    neighbor3 = doc_que[doc_id][2];
    fprintf(stdout, "%d %d %d %d %d %d %d %s\n", 
	   doc_id, 
	   suffix_pos, 
	   suffix_id,
	   lcp,
	   neighbor1,
	   neighbor2,
	   neighbor3,
	   text);
    doc_que[doc_id][2] = doc_que[doc_id][1];
    doc_que[doc_id][1] = doc_que[doc_id][0];
    doc_que[doc_id][0] = suffix_id;
    suffix_id++;
  }
}
