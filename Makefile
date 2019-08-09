make:
	gcc -c -g shm.c -o shm.o
	gcc -c -g hashtable.c -o hashtable.o
	gcc -c -g Semun.c -o Semun.o
	gcc -c -g main.c -o main.o
	gcc -Wall -g main.o shm.o Semun.o hashtable.o -o main

run:
	./main 1

val:
	valgrind --leak-check=yes ./main 1

clean:
	rm *.o
	rm main