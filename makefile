CC = g++
SRC = main.cpp
PROG = elevator
STD = -std=c++17

$(PROG): $(SRC)
	$(CC) $(STD) $(SRC) -o $(PROG)

clean:
	rm elevator *.o *.out
