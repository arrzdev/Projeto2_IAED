compile:
	gcc -Wall -Wextra -Werror -ansi -pedantic -o proj2 ./src/*.c
	
soft-compile:
	gcc -o proj2 ./src/*.c	

gdb-compile:
	gcc -g -o proj2 ./src/*.c
	
run:
	gcc -Wall -Wextra -Werror -ansi -pedantic -o proj2 ./src/*.c
	echo "\033[1;33m1 PROJECT MOOSHAK TESTS\033[0m"
	./test.sh -d ./proj2 ./proj1-mooshak
	echo "\033[1;33mPUBLIC TESTS\033[0m"
	./test.sh -d ./proj2 ./proj2-public-tests
	echo "\033[1;33mCOMMUNITY TESTS\033[0m"
	./test.sh -d ./proj2 ./community_tests
	echo "\033[1;33mEXTENSIVE TESTS\033[0m"
	./test.sh -d ./proj2 ./extensive_tests
