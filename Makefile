output: lab2_add.c
	gcc -Wall -Wextra -pthread lab2_add.c -o lab2_add

debug: lab2_add.c
	gcc -Wall -Wextra -pthread -g lab2_add.c -o lab2_add

clean:
	rm -f *.o
	rm -f lab2_add
	rm -f *.gz
	rm -f *.txt

dist: 
	tar -czf lab2a-40205638.tar.gz lab2_add.c Makefile README

# check: 
	