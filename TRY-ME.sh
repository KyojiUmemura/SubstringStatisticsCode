mkdir -p ../TRY
cp CORPUS ../TRY
./s1-generate-suffix       < ../TRY/CORPUS > ../TRY/CORPUS.s1
sh ./s2-sort-suffix.sh     < ../TRY/CORPUS.s1 > ../TRY/CORPUS.s2
./s3-compute-neighbor      < ../TRY/CORPUS.s2 > ../TRY/CORPUS.s3
./s4-compute-lcp           < ../TRY/CORPUS.s3 > ../TRY/CORPUS.s4
./s5-detect-class          < ../TRY/CORPUS.s4 > ../TRY/RESULT
