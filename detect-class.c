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
{ int id;
  int start;
  int sil;
  int end;
  int lbl;
  int cdf1;
  int cdf2;
  int cdf3;
  int cdf4; };

#define CLASS_MAX 10240
int class_size=0;
int class_id = 0;
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

static int lcp_length(char *x, char *y)
{  
  int r;
  r = 0;
  while((*x != '\n') && (*y != '\n') && ( *x == *y)) {
    x++; y++; r++;
  }
  return r;
}

char lcp_character(int suffix, int k)
{ 
  if(k == suffix_table[suffix].lcp) return '.';
  return ' ';
}
 

char class_character(int suffix, int k)
{ 
 int i;
 for(i=0;i<class_size;i++) {
   if((class_table[i].start == suffix)&&(class_table[i].sil==k)) return '.';
   if((class_table[i].end   == suffix)&&(class_table[i].sil==k)) return ';';
   if((class_table[i].sil == k) && (class_table[i].start < suffix) && (class_table[i].end > suffix)) return ':';
 }
 return ' ';
}

char print_lcp_line(int n)
{int j;
 for(j=0;j<4*7;j++) putchar(' ');
 for(j=0;j<n;j++) printf("  ");
 printf("+\n");
}

char class_contents(int suffix)
{
  int i; int j; int lbl; int k;
  for(i=0;i<class_size;i++) {
    if((class_table[i].end == suffix)) {
      for(j=0;j<4*7;j++) putchar(' ');
      for(j=0;j<class_table[i].sil;j++) {
	if(j == suffix_table[suffix].lcp) { printf(". "); } else { printf("  "); }
      }
      lbl = suffix_table[class_table[i].end].lcp;
      if((class_table[i].start >= 0) &&
	 (lbl < suffix_table[class_table[i].start - 1].lcp))
	lbl = suffix_table[class_table[i].start - 1].lcp;
      putchar('^');
      /*      printf("LBL=%d ", lbl); */
      for(j=class_table[i].sil;j>lbl;j--) {
	putchar('\"');
	for(k=0;k<j;k++) { putchar(text[suffix_table[class_table[i].start].suffix_pos+k]); }
	putchar('\"');
	putchar(',');
      }
      printf("[%d,%d] ", class_table[i].start, class_table[i].end);
      printf("CDF1=%d CDF2=%d CDF3=%d ", class_table[i].cdf1, class_table[i].cdf2, class_table[i].cdf3);
      putchar('\n');
    }
  }
}

void show_class_on_suffix(void)
{struct suffix_struct * s; char * p;
 int i; int j;
 for(i=0;i<suffix_size;i++) {
   s = &suffix_table[i];
   printf("%3d %3d %3d %3d %3d %3d %3d ",
	  s->doc_id, s->suffix_pos, s->suffix_id, s->lcp,
	  s->neighbor1, s->neighbor2, s->neighbor3);
   p = &text[s->suffix_pos];
   j = 0;
   for(;;) {
     /* putchar(class_character(i,j));  */
     putchar(lcp_character(i,j));
     if(p[j] == '\n') break;
     putchar(p[j]); 
     j++;
   };
   putchar('\n');
   class_contents(i);
   /*   print_lcp_line(s->lcp); */
 }
}


void print_suffix(int i)
{ struct suffix_struct * s; char * p;
  s = &suffix_table[i];
  printf("%d %d %d %d %d %d %d ",
         s->doc_id, s->suffix_pos, s->suffix_id, s->lcp,
         s->neighbor1, s->neighbor2, s->neighbor3);
  p = &text[s->suffix_pos];
  while(*p != '\n') { putchar(*p); p++; };
  putchar('\n');
}

void print_class(int n)
{ struct class_struct * c; char * p; int i;
  c = &class_table[n];
  printf("%d %d %d %d %d %d %d %d ",
	 c->id, c->start, c->end, c->sil, c->cdf1, c->cdf2, c->cdf3, c->cdf4);
  p = &text[suffix_table[c->start].suffix_pos];
  for(i=0;i<c->sil;i++) { putchar(p[i]); };
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
    suffix_table[i].lcp = lcp_length(&text[suffix_table[i].suffix_pos], &text[suffix_table[i+1].suffix_pos]);
  }
  suffix_table[suffix_size-1].lcp = 0;
#ifdef DEBUG
  for(i=0;i<suffix_size;i++) { print_suffix(i); };
#endif

  class_size = 0;
  /* create root class */
  current_level = 0;
  class_path[current_level].id = class_id++;
  class_path[current_level].start = 0;
  class_path[current_level].sil   = -1;
  for(i=0;i<suffix_size;i++) {
    struct suffix_struct *s;
    s = &suffix_table[i];
    if(s->lcp> class_path[current_level].sil) { 
      if(current_level >= PATH_MAX) { 	fprintf(stderr, "PATH_MAX\n"); exit(1); }
      current_level++; 
      class_path[current_level].id = class_id++;
      class_path[current_level].start = s->suffix_id;
      class_path[current_level].sil = s->lcp;
      class_path[current_level].cdf1 = 0;
      class_path[current_level].cdf2 = 0;
      class_path[current_level].cdf3 = 0;
      class_path[current_level].cdf4 = 0; }
    /* increment responsible cdf **/
    class_path[current_level].cdf1++;
    if(s->neighbor1 >= 0) {
      level1 = find_responsible_class_level(s->neighbor1);
      class_path[level1].cdf2++;
      if(s->neighbor2 >= 0) {
	level2 = find_responsible_class_level(s->neighbor2);
	class_path[level2].cdf3++;
	if(s->neighbor3 >= 0) {
	  level3 = find_responsible_class_level(s->neighbor3);
	  class_path[level3].cdf4++; } } }
    /* while a class is terminating, report the class, */
    /* and propagate cdf to its super class */
    while(s->lcp < class_path[current_level].sil) {
      class_path[current_level].end = s->suffix_id;
      class_table[class_size] = class_path[current_level];
      print_class(class_size);
      class_size ++;
      if(current_level <=0 ) { fprintf(stderr, "internal\n"); exit(1); }
      /* when the implicitly started class is found, */
      /* replace the last termnating class to it. */
      if(s->lcp > class_path[current_level-1].sil) {
	class_path[current_level].id = class_id++;
	class_path[current_level].sil = s->lcp; }
      /* when no implicitly started class is found, */
      /* prepagate cdfs to super class */
      if(s->lcp <= class_path[current_level-1].sil){
	class_path[current_level-1].cdf1 += class_path[current_level].cdf1;
	class_path[current_level-1].cdf2 += class_path[current_level].cdf2;
	class_path[current_level-1].cdf3 += class_path[current_level].cdf3;
	class_path[current_level-1].cdf4 += class_path[current_level].cdf4;
	current_level--; } 
    } 
  } 
}
