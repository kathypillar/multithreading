CC = g++

main: main.o
	$(CC) -o sortingTestbench main.o
	./sortingTestbench
	nano data.txt
test: main.o
	$(CC) -o sortingTestbench main.o
	./sortingTestbench
	nano test.txt
main.o: main.cpp
run:
	./sortingTestbench
clean:
	$(RM) main.o sortingTestbench
