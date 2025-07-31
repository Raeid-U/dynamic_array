.PHONY:
	clean da leaks

da: da.c lumber.h
	gcc -o da da.c

leaks: da
	valgrind --leak-check=full ./da

clean: 
	rm -rf ./da && rm -rf logs
