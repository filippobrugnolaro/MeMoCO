# MeMoCO
Exercises for the Methods and Models for Combinatorial Optimisation course at the University of Padova.

## Problem overview
A company produces boards with holes used to build electric panels. Boards
are positioned over a machine and a drill moves over the board, stops at the desired
positions and makes the holes. Once a board is drilled, a new board is positioned and
the process is iterated many times. Given the position of the holes on the board, the
company asks us to determine the hole sequence that minimizes the total drilling time,
taking into account that the time needed for making an hole is the same and constant for
all the holes.

## Mathematical formulation
We can represent the problem on a weighted complete graph G = (N, A), where N is the
set of nodes and corresponds to the set of the positions where holes have to be made, and
A is the set of the arcs (i, j), ∀ i, j ∈ N , corresponding to the trajectory of the drill moving
from hole i to hole j. A weight c<sub>ij</sub> can be associated to each arc (i, j) ∈ A, corresponding
to the time needed to move from i to j. In this graph model, the problem can be seen
as finding the path of minimum weight that visits all the nodes. Indeed, since the drill
has to come back to the initial position in order to start with the next board, the path
has to be a cycle. The problem can be thus seen as determining the minimum weight
hamiltonian cycle on G, that is a Travelling Salesman Problem (TSP) on G.

## Objective
The aim is to implement an exact and a metaheuristc method in order to try to solve the TSP problem,
which is know to be NP-hard. For the first CPLEX APIs was used, while for the second a Tabu search
was implemented, trying to reduce the execution time of the CPLEX method and to obtain a suboptimal
solution.
