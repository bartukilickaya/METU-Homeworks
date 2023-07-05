all: hw2

hw2: hw2.c hw2_output.c hw2_output.h
	gcc hw2.c hw2_output.c -lpthread -o hw2

clean:
	rm -f hw2

valgrind_memcheck: hw2
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./hw2

valgrind_deadlock: hw2
	valgrind --tool=helgrind --read-var-info=yes ./hw2