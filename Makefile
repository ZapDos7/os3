make:
	gcc -c -g shm.c -o shm.o
	gcc -c -g Semun.c -o Semun.o
	gcc -c -g main.c -o main.o
	#gcc md5.c -o md5 -lssl
	gcc -Wall -g main.o shm.o Semun.o -o main

run:
	./main 2

val:
	valgrind --leak-check=yes ./main 1

clean:
	rm *.o
	rm main