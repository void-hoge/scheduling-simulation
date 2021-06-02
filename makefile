CC = g++-11
SRC = main.cpp
PROG = elevator
STD = -std=c++17
OPTIM = -O2 -mtune=native -march=native

$(PROG): $(SRC)
	$(CC) $(STD) $(SRC) -o $(PROG)

multiprocess: multiprocess.cpp
	$(CC) $(STD) multiprocess.cpp -o multiprocess

clean:
	rm multiprocess elevator *.o *.out
