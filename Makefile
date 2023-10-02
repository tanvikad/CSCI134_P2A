output: lab2_add.c lab2_list.c 
	gcc -Wall -Wextra -pthread lab2_add.c -o lab2_add
	gcc -Wall -Wextra -pthread lab2_list.c SortedList.c -o lab2_list


debug: lab2_add.c
	gcc -Wall -Wextra -pthread -g lab2_add.c -o lab2_add
	gcc -Wall -Wextra -pthread -g lab2_list.c SortedList.c -o lab2_list


graphs:
	gnuplot lab2_add.gp
	gnuplot lab2_list.gp

tests:
	# for its in 10 20 40 80 100 1000 10000 100000 ; do \
	# 	for thr in 1 2 4 8 12 ;	 do \
	# 		./lab2_add --threads $$thr --iterations $$its ; \
	# 		./lab2_add --threads $$thr --iterations $$its --yield ; \
	# 	done \
	# done

	# for its in 100000 1000000 10000000 100000000 ; do \
	# 	./lab2_add --threads 1 --iterations $$its ; \
	# done

	# for thr in 1 2 4 8 12 ; do \
	# 	./lab2_add --threads $$thr --iterations 10000  --yield --sync=m ; \
	# 	./lab2_add --threads $$thr --iterations 1000  --yield --sync=s ; \
	# 	./lab2_add --threads $$thr --iterations 10000 --yield --sync=s ; \
	# 	./lab2_add --threads $$thr --iterations 10000  --yield --sync=c ; \
	# 	./lab2_add --threads $$thr --iterations 10000 --sync=m ; \
	# 	./lab2_add --threads $$thr --iterations 1000 --sync=s ; \
	# 	./lab2_add --threads $$thr --iterations 10000 --sync=s ; \
	# 	./lab2_add --threads $$thr --iterations 10000 --sync=c ; \
	# done

	for its in 10 100 1000 10000 20000 ; do \
		./lab2_list --threads 1 --iterations $$its ; \
	done	

	-for its in 1 10 100 1000 ; do \
		for thr in 2 4 8 12 ; do \
			./lab2_list --threads $$thr --iterations $$its ; \
		done \
	done

	-for its in 1 2 4 8 16 32 ; do \
		for thr in 2 4 8 12 ; do \
			./lab2_list --threads $$thr --iterations $$its --yield=i ; \
			./lab2_list --threads $$thr --iterations $$its --yield=d ; \
			./lab2_list --threads $$thr --iterations $$its --yield=il ; \
			./lab2_list --threads $$thr --iterations $$its --yield=dl ; \
			./lab2_list --threads $$thr --iterations $$its --yield=i --sync=m; \
			./lab2_list --threads $$thr --iterations $$its --yield=d --sync=m; \
			./lab2_list --threads $$thr --iterations $$its --yield=il --sync=m; \
			./lab2_list --threads $$thr --iterations $$its --yield=dl --sync=m; \
			./lab2_list --threads $$thr --iterations $$its --yield=i --sync=s; \
			./lab2_list --threads $$thr --iterations $$its --yield=d --sync=s; \
			./lab2_list --threads $$thr --iterations $$its --yield=il --sync=s; \
			./lab2_list --threads $$thr --iterations $$its --yield=dl --sync=s; \
		done \
	done

	-for its in 1000 ; do \
		for thr in 2 4 8 12 16 24 ; do \
			./lab2_list --threads $$thr --iterations $$its --sync=m ; \
			./lab2_list --threads $$thr --iterations $$its --sync=s ; \
			./lab2_list --threads $$thr --iterations $$its ; \
		done \
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
	