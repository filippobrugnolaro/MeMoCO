CC = g++
CPPFLAGS = -g -Wall -O2
LDFLAGS =

OBJ = TSPSolver.o TSPSolution.o TSPMove.o TSP.o customTimer.o main.o

%.o: %.cpp
		$(CC) $(CPPFLAGS) -c $^ -o $@

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o main 
		
clean:
		rm -rf $(OBJ) main

.PHONY: clean
