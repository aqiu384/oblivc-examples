../../../bin/oblivcc sha256.c sha256_test.c
../cycle './a.out 1 "./text/text1.txt"| ./a.out 2 "./text/text1.txt"'
