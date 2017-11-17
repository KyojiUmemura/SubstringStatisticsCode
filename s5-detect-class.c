/* This program compute classes and their cfs*/
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

/* To make this program as simple as possible, the program assumes that
   the maximun length of documents is less than 10240. It also assumes */
/* the contents consists of graphical_character (i.e. not space or control)*/
/* The work around is easy, but make the program hard to read */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* These variables are updated by input, line by line */
static int doc_id;
static int suffix_pos;
static int suffix_id;
static int lcp;
static int neighbor1;
static int neighbor2;
static int neighbor3;
static char text[10240];
static char line[10240];
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
/* Path is a sequence of class from root class to the current class */
/* The length of path may becomes as large as length of one documents. */
/* When all characters in the documents are identical, it is the worst case. */
#define PATH_MAX 10240
static int current_level = 0;
struct class_struct
{ int start;
  int sil;
  int cdf1;
  int cdf2;
  int cdf3;
  int cdf4; };
static struct class_struct class_path[PATH_MAX];
static int level1, level2, level3;
static int find_responsible_class_level(int id)
{ /* Binary search could be used because class_path are ordere by "start" */
  int i;
  for(i=current_level;i>=0;i--) {
    if(class_path[i].start <= id) return i; }
  fprintf(stderr, "Internal error\n");
  exit(1); }
int main()
{ 
  /* create root class */
  current_level = 0;
  class_path[current_level].start = 0;
  class_path[current_level].sil   = -1;
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
	current_level--; } } } }
