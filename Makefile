.PHONY:
	clean main leaks

main: main.c da.c lumber.h
	gcc -Wall -Wextra -DLOGGING main.c da.c -o main 

leaks: main
	valgrind --leak-check=full ./main

clean: 
	rm -rf ./main && rm -rf logs
