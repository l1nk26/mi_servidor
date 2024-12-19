compile:
	gcc main.c server.c -Wall -o server -lws2_32

run: compile
	./server

clean:
	rm server

