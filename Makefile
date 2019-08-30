make:
	gcc -c -g shm.c -o shm.o
	gcc -c -g Semun.c -o Semun.o
	gcc -c -g main.c -o main.o
	gcc -c -g help.c -o help.o
	#-lssl & -lcrypto for md5
	gcc -Wall -g main.o shm.o Semun.o help.o -o main -lcrypto -lssl 

run:
	./main 2 3

val:
	valgrind --leak-check=yes ./main 2 3

clean:
	rm *.o
	rm main