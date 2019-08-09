make:
	gcc -o main main.c

run:
	./main 1

val:
	valgrind --leak-check=yes ./main 1

clean:
#	rm *.o
	rm main