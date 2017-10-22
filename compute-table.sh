#! /bin/bash
./s1-generate-suffix | sh s2-sort-suffix.sh | ./s3-compute-neighbor | ./s4-compute-lcp