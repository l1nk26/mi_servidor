compile:
	gcc main.c -Wall -o server -lws2_32

run: compile
	./server

clean:
	rm server

