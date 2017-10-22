/* $1: document_id                                (doc_id)*/
/* $2: position_in the corpus                 (suffix_pos)*/
/* $3: suffix sequence number                  (suffix_id)*/
/* $4: length of longest common prefix               (lcp)*/
/* $5: doc_link(suffix_id)                           (dl1)*/
/* $6: doc_link(doc_link(suffix_id))                 (dl2)*/
/* $7: doc_link(doc_link(doc_link(suffix_id)))       (dl3)*/
/* $8: contents at the position  */
/* -1 means, the value is not valid */
#include <stdio.h>
#include <ctype.h>
static char line[1024];
static char suffix[1024];
static int doc_id = -1;
static int suffix_pos = -1;
static int suffix_id = -1;
static int lcp = -1;
static int dl1 = -1;
static int dl2 = -1;
static int dl3 = -1;
static int phai = -1;
main()
{ char * p; int n; 
  fprintf(stdout, "%10s %10s %10s %10s %10s %10s %10s %s\n", 
	    "doc_id", "suffix_pos", "suffix_id", "lcp",
	    "neighbor1", "neighbor2", "neighbor3", "contents");
  suffix[0] = 0;
  while(fgets(line, sizeof(line), stdin)) {
    suffix[0]=0;
    sscanf(line, "%d %d %d %d %d %d %d %s", 
	   &doc_id, 
	   &suffix_pos, 
	   &suffix_id,
	   &lcp,
	   &dl1,
	   &dl2,
	   &dl3,
	   suffix);
    fprintf(stdout, "%10d %10d %10d %10d %10d %10d %10d %s\n", 
	    doc_id, suffix_pos, suffix_id, lcp,
	    dl1, dl2, dl3, suffix);
    suffix[0]=0;
  }
}
      
      
      
    
    
    
