.PHONY:
	clean main leaks

main: main.c da.c lumber.h
	gcc -o main main.c da.c

leaks: main
	valgrind --leak-check=full ./main

clean: 
	rm -rf ./main && rm -rf logs
