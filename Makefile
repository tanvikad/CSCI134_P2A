output: lab2_add.c lab2_list.c 
	gcc -Wall -Wextra -pthread lab2_add.c -o lab2_add
	gcc -Wall -Wextra -pthread lab2_list.c SortedList.c -o lab2_list


debug: lab2_add.c
	gcc -Wall -Wextra -pthread -g lab2_add.c -o lab2_add
	gcc -Wall -Wextra -pthread -g lab2_list.c SortedList.c -o lab2_list


tests:
	for its in 10 20 40 80 100 1000 10000 100000 ; do \
		for thr in 1 2 4 8 12 ; do \
			./lab2_add --threads $$thr --iterations $$its ; \
			./lab2_add --threads $$thr --iterations $$its --yield ; \
		done \
	done

	for its in 100000 1000000 10000000 100000000 ; do \
		./lab2_add --threads 1 --iterations $$its ; \
	done

	for thr in 1 2 4 8 12 ; do \
		./lab2_add --threads $$thr --iterations 10000  --yield --sync=m ; \
		./lab2_add --threads $$thr --iterations 1000  --yield --sync=s ; \
		./lab2_add --threads $$thr --iterations 10000 --yield --sync=s ; \
		./lab2_add --threads $$thr --iterations 10000  --yield --sync=c ; \
		./lab2_add --threads $$thr --iterations 10000  --yield ; \
		./lab2_add --threads $$thr --iterations 10000  ; \
		./lab2_add --threads $$thr --iterations 10000 --sync=m ; \
		./lab2_add --threads $$thr --iterations 1000 --sync=s ; \
		./lab2_add --threads $$thr --iterations 10000 --sync=s ; \
		./lab2_add --threads $$thr --iterations 10000 --sync=c ; \
	done

clean:
	rm -f *.o
	rm -f lab2_add
	rm -f lab2_list
	rm -f *.gz
	rm -f *.txt

dist: 
	tar -czf lab2a-40205638.tar.gz lab2_add.c Makefile README

# check: 
	