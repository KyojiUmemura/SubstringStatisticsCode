./s1-generate-suffix       < CORPUS > CORPUS.s1
sh ./s2-sort-suffix.sh     < CORPUS.s1 > CORPUS.s2
./s3-compute-neighbor      < CORPUS.s2 > CORPUS.s3
./s4-compute-lcp           < CORPUS.s3 > CORPUS.s4
./s5-detect-class          < CORPUS.s4 > RESULT
