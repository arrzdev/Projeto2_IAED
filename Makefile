compile:
	gcc -Wall -Wextra -Werror -ansi -pedantic -o proj2 ./src/*.c
	
soft-compile:
	gcc -o proj2 ./src/*.c	
	
run:
	gcc -Wall -Wextra -Werror -ansi -pedantic -o proj2 ./src/*.c
	python3 tester.py