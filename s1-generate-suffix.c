/* This program convert byte sequence to internal structure to compute cummulated document frequency */
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
#define MASK 0x3f
static int ring_buffer[MASK+1];
static int doc_id = -1;
static int suffix_pos = -1;
static int suffix_id = -1;
static int lcp = -1;
static int neighbor1 = -1;
static int neighbor2 = -1;
static int neighbor3 = -1;

static void suffix_out(int pos)
{ int i;
  if(pos < 0) return;
  if(ring_buffer[pos & MASK] == 0) return;
  fprintf(stdout, "%d %d %d %d %d %d %d ", 
	  doc_id, 
	  pos,
	  suffix_id,
	  lcp,
	  neighbor1,
	  neighbor2,
	  neighbor3);
  if(ring_buffer[pos & MASK] == '\n'){
    doc_id ++;
  }
  for(i=0;i<MASK+1;i++) {
    if(ring_buffer[(pos + i) & MASK] == 0) break;
    if(ring_buffer[(pos + i) & MASK] == '\n') break;
    if(isgraph(ring_buffer[(pos + i) & MASK])){
      fprintf(stdout, "%c", ring_buffer[(pos + i) & MASK]);
    } else {
      fprintf(stdout, "_");
    }
  }
  fprintf(stdout, "\n");
}


int main()
{
  int i; int ch; int count; 
  for(i=0;i<MASK+1;i++) ring_buffer[i] = 0;
  count = 0;
  doc_id = 0;
  while(EOF != (ch = getchar())) {
    ring_buffer[count & MASK] = ch;
    count ++;
    suffix_out(count - MASK - 1);
  }
  for(i=0;i<MASK + 1; i++) {
    ring_buffer[count & MASK] = 0;
    count ++;
    suffix_out(count - MASK -1);
  }
}      
