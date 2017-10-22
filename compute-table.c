/* This program shows to what is class */
/* The input line consists of 8 column : Enhanced Suffix arary */
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

#include <stdio.h>
#include <ctype.h>
#define TEXT_MAX 10240
static int text_size=0;
char text[TEXT_MAX];
static int current_doc=0;

/* These variables are updated by input, line by line */
struct suffix_struct {
  int doc_id;
  int suffix_pos;
  int suffix_id;
  int lcp;
  int neighbor1;
  int neighbor2;
  int neighbor3;
};


struct suffix_struct current_suffix;

#define SUFFIX_MAX 10240
int    suffix_size = 0;
struct suffix_struct suffix_table[SUFFIX_MAX];

static char line[1024];
/* printing class: a set of string whose statistics are identical */
static void report_class(int first, int last, int sil, 
			 int cdf1, int cdf2, int cdf3, int cdf4)
{ int i;
  fprintf(stdout, "CLASS[%d, %d]: L=%d CDF1=%d CDF2=%d CDF3=%d CDF4=%d ", 
	  first, last, sil, cdf1, cdf2, cdf3, cdf4);
  fprintf(stdout, "df=%d df2=%d df3=%d ", cdf1 - cdf2, cdf2 - cdf3, cdf3 - cdf4);
  fprintf(stdout, "S=");
  for(i=0;i<sil;i++) { fprintf(stdout, "%c", text[i]); }
  fprintf(stdout, "\n");
  fflush(stdout); }
/* classs is the set of string, that share the occurence pattern */

struct class_struct
{ int start;
  int sil;
  int end;
  int lbl;
  int cdf1;
  int cdf2;
  int cdf3;
  int cdf4; };

#define CLASS_MAX 10240
int class_size=0;
struct class_struct class_table[CLASS_MAX];

/* Path is a sequence of class from root class to the current class */
/* The length of path may becomes as large as length of one documents. */
/* When all characters in the documents are identical, it is the worst case. */
#define PATH_MAX 10240
static int current_level = 0;

static struct class_struct class_path[PATH_MAX];
static int level1, level2, level3;
static int find_responsible_class_level(int id)
{ /* Binary search could be used because class_path are ordere by "start" */
  int i;
  for(i=current_level;i>=0;i--) {
    if(class_path[i].start <= id) return i; }
  fprintf(stderr, "Internal error\n");
  exit(1);  }

static int suffix_text_order(void *x, void * y)
{ int r;
  struct suffix_struct * xs;
  struct suffix_struct * ys;
  char * xt;
  char * yt;
  xs = (struct suffix_struct *) x;
  ys = (struct suffix_struct *) y;
  xt = &text[xs->suffix_pos];
  yt = &text[ys->suffix_pos];
  while((*xt != '\n') && (*yt != '\n') && (*xt == *yt)) { xt++; yt++;}
  if(*xt<*yt) return -1;
  if(*xt>*yt) return  1;
  if(*xt==*yt) return 0;
}

static int suffix_document_text_order(void *x, void * y)
{ int r;
  struct suffix_struct * xs;
  struct suffix_struct * ys;
  char * xt;
  char * yt;
  xs = (struct suffix_struct *) x;
  ys = (struct suffix_struct *) y;
  if(xs->doc_id < ys->doc_id) return -1;
  if(xs->doc_id > ys->doc_id) return  1;
  xt = &text[xs->suffix_pos];
  yt = &text[ys->suffix_pos];
  while((*xt != '\n') && (*yt != '\n') && (*xt == *yt)) { xt++; yt++;}
  if(*xt<*yt) return -1;
  if(*xt>*yt) return  1;
  if(*xt==*yt) return 0;
}

static int lcp(char *x, char *y)
{  
  int r;
  r = 0;
  while((*x != '\n') && (*y != '\n') && ( *x == *y)) {
    x++; y++; r++;
  }
  return r;
}

void print_suffix(int i)
{ struct suffix_struct * s; char * p;
  s = & suffix_table[i];
  printf("%d %d %d %d %d %d %d ",
         s->doc_id, s->suffix_pos, s->suffix_id, s->lcp,
         s->neighbor1, s->neighbor2, s->neighbor3);
  p = &text[s->suffix_pos];
  while(*p != '\n') { putchar(*p); p++; };
  putchar('\n');
}
  

main()
{ 
  int ch;
  int i;
  int n1, n2, n3;
  text_size = 0;
  while(EOF != (ch = getchar())) {
    if(text_size >= TEXT_MAX) { exit(1); };
    text[text_size] = ch; 
    text_size++; 
  }
  suffix_size = 0;
  current_doc = 0;
  for(i=0;i<text_size;i++) {
    suffix_table[i].doc_id = current_doc;
    suffix_table[i].suffix_pos = i;
    if(text[i]=='\n') current_doc++;
    suffix_size++;
  }
  qsort(suffix_table, suffix_size, sizeof(struct suffix_struct), suffix_text_order);
  for(i=0;i<suffix_size;i++) {
    suffix_table[i].suffix_id = i;
  }
  qsort(suffix_table, suffix_size, sizeof(struct suffix_struct), suffix_document_text_order);
  current_doc = -1;
  for(i=0;i<suffix_size;i++) {
    if(suffix_table[i].doc_id != current_doc) {
      current_doc = suffix_table[i].doc_id;
      n1 = n2 = n3 = -1;
    }
    suffix_table[i].neighbor3 = n3;
    n3 = suffix_table[i].neighbor2 = n2;
    n2 = suffix_table[i].neighbor1 = n1;
    n1 = suffix_table[i].suffix_id;
  }
  qsort(suffix_table, suffix_size, sizeof(struct suffix_struct), suffix_text_order);
  for(i=0;i<suffix_size-1;i++) {
    suffix_table[i].lcp = lcp(&text[suffix_table[i].suffix_pos], &text[suffix_table[i+1].suffix_pos]);
  }
  suffix_table[suffix_size-1].lcp = 0;
  for(i=0;i<suffix_size;i++) { print_suffix(i); };
  current_level = 0;


#ifdef DEBUG  
  while(fgets(line, sizeof(line), stdin)) {
    /* parse field, and update input variables */
    sscanf(line, "%d %d %d %d %d %d %d %s", 
	   &doc_id,  &suffix_pos,  &suffix_id, &lcp,  
	   &neighbor1,  &neighbor2, &neighbor3, text);
    /* when new class starts, prepare new class structure*/
    if(lcp> class_path[current_level].sil) { 
      if(current_level >= PATH_MAX) { 	fprintf(stderr, "PATH_MAX\n"); exit(1); }
      current_level++; 
      class_path[current_level].start = suffix_id;
      class_path[current_level].sil = lcp;
      class_path[current_level].cdf1 = 0;
      class_path[current_level].cdf2 = 0;
      class_path[current_level].cdf3 = 0;
      class_path[current_level].cdf4 = 0; }
    /* increment responsible cdf **/
    class_path[current_level].cdf1++;
    if(neighbor1 >= 0) {
      level1 = find_responsible_class_level(neighbor1);
      class_path[level1].cdf2++;
      if(neighbor2 >= 0) {
	level2 = find_responsible_class_level(neighbor2);
	class_path[level2].cdf3++;
	if(neighbor3 >= 0) {
	  level3 = find_responsible_class_level(neighbor3);
	  class_path[level3].cdf4++; } } }
    /* while a class is terminating, report the class, */
    /* and propagate cdf to its super class */
    while(lcp < class_path[current_level].sil) {
      report_class( class_path[current_level].start,
		    suffix_id,
		    class_path[current_level].sil,
		    class_path[current_level].cdf1,
		    class_path[current_level].cdf2,
		    class_path[current_level].cdf3,
		    class_path[current_level].cdf4);
      if(current_level <=0 ) { fprintf(stderr, "internal\n"); exit(1); }
      /* when the implicitly started class is found, */
      /* replace the last termnating class to it. */
      if(lcp > class_path[current_level-1].sil) {
	class_path[current_level].sil = lcp; }
      /* when no implicitly started class is found, */
      /* prepagate cdfs to super class */
      if(lcp <= class_path[current_level-1].sil){
	class_path[current_level-1].cdf1 += class_path[current_level].cdf1;
	class_path[current_level-1].cdf2 += class_path[current_level].cdf2;
	class_path[current_level-1].cdf3 += class_path[current_level].cdf3;
	class_path[current_level-1].cdf4 += class_path[current_level].cdf4;
	current_level--; } 
    } 
  } 
#endif
}
