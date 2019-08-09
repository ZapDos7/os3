make:
	gcc -c -g shm.c -o shm.o
	gcc -c -g main.c -o main.o
	gcc -Wall -g main.o shm.o -o main

run:
	./main 1

val:
	valgrind --leak-check=yes ./main 1

clean:
	rm *.o
	rm main