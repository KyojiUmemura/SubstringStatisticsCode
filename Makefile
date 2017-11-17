all: show-class show-suffix s1-generate-suffix s3-compute-neighbor s4-compute-lcp s5-detect-class detect-class

show-class: show-class.c
	gcc -Wall -o show-class show-class.c
show-suffix: show-suffix.c
	gcc -Wall -o show-suffix show-suffix.c
s1-generate-suffix: s1-generate-suffix.c
	gcc -Wall -o s1-generate-suffix s1-generate-suffix.c
s3-compute-neighbor: s3-compute-neighbor.c
	gcc -Wall -o s3-compute-neighbor s3-compute-neighbor.c
s4-compute-lcp: s4-compute-lcp.c
	gcc -Wall -o s4-compute-lcp s4-compute-lcp.c
s5-detect-class: s5-detect-class.c
	gcc -Wall -o s5-detect-class s5-detect-class.c

detect-class: detect-class.c
	gcc -Wall -o detect-class detect-class.c
clean:
	rm -f show-class show-suffix s1-generate-suffix show-suffix s3-compute-neighbor s4-compute-lcp s5-detect-class detect-class RESULT CORPUS.s1 CORPUS.s2 CORPUS.s3 CORPUS.s4 *~

