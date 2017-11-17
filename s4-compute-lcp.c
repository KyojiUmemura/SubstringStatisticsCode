/* This program compute lcp, (length of longest common prefix) of each suffix */
/* $1: document_id                                (doc_id)*/
/* $2: position_in the corpus                 (suffix_pos)*/
/* $3: suffix sequence number                  (suffix_id)*/
/* $4: length of longest common prefix               (lcp)*/
/* $5: the first_upper neighbor                (neighbor1)*/
/* $6: the second upper neighbor               (neighbor2)*/
/* $7: the third upper neibghor                (neighbor3)*/
/* $8: contents at the position     (text)     */
/* Note: $8 is used to understand classes, not used to compute classs or cdf */
/* Suffix's Neighbor: 
      position of other suffix of the same documents,
      whose position on suffix array is close to each other */
/* -1 means, the value is not valid */


#include <stdio.h>
#include <ctype.h>
#define MAXDOC 10240
static char line[1024];
static char text1[1024];
static char text2[1024];
static int doc_que[MAXDOC][3];
static int doc_id = -1;
static int text_pos = -1;
static int text_id = -1;
static int lcp = -1;
static int neighbor1 = -1;
static int neighbor2 = -1;
static int neighbor3 = -1;
static char * text;

static int new_doc_id = -1;
static int new_text_pos = -1;
static int new_text_id = -1;
static int new_lcp = -1;
static int new_neighbor1 = -1;
static int new_neighbor2 = -1;
static int new_neighbor3 = -1;
static char * new_text;

int lcp_string(char * s1, char * s2)
{ 
  int r;
  r = 0;
  while(s1[0] == s2[0]){
    if(s1[0]== 0) break;
    r ++;
    s1 += 1;
    s2 += 1;
  }
  return r;
}
 
  

int main()
{ char * p; int n; int i; int j; int dummy; char * temp_text;
 
  text = text1;
  new_text = text2;

  text_id = 0;
  if(fgets(line, sizeof(line), stdin)) {
    sscanf(line, "%d %d %d %d %d %d %d %s", 
	   &doc_id, 
	   &text_pos, 
	   &text_id,
	   &lcp,
	   &neighbor1,
	   &neighbor2,
	   &neighbor3,
	   text);
    while(fgets(line, sizeof(line), stdin)) {
      sscanf(line, "%d %d %d %d %d %d %d %s", 
	     &new_doc_id, 
	     &new_text_pos, 
	     &new_text_id,
	     &new_lcp,
	     &new_neighbor1,
	     &new_neighbor2,
	     &new_neighbor3,
	     new_text);
      lcp = lcp_string(new_text, text); 
      fprintf(stdout, "%d %d %d %d %d %d %d %s\n", 
	   doc_id, 
	   text_pos, 
	   text_id,
	   lcp,
	   neighbor1,
	   neighbor2,
	   neighbor3,
	   text);
      doc_id = new_doc_id;
      text_pos = new_text_pos;
      text_id = new_text_id;
      lcp = new_lcp;
      neighbor1 = new_neighbor1;
      neighbor2 = new_neighbor2;
      neighbor3 = new_neighbor3;
      temp_text = text;
      text = new_text;
      new_text = temp_text;
    }
    lcp = 0;
    fprintf(stdout, "%d %d %d %d %d %d %d %s\n", 
	   doc_id, 
	   text_pos, 
	   text_id,
	   lcp,
	   neighbor1,
	   neighbor2,
	   neighbor3,
	   text);
  }
}
      
      
      
    
    
    
