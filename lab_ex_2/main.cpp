/**
 * @file main.cpp
 * @brief
 */

#include <stdexcept>

#include "TSPSolver.h"
#include "customTimer.h"

// error status and messagge buffer
int status;
char errmsg[255];

int main(int argc, char const* argv[]) {
    try {
        /*
            ./main filename.dat tabulength maxiter init [readPos] [N]

            filename.dat:
            - costs.dat:    argc == 5
            - pos.dat:      argc == 6
            - not used:     argc == 7

            init: enable initial heuristic solution
            - 0: random
            - != 0: heuristic nearest neighbour

            readPos: file to read positions

            N: Number of nodes
        */

        if (argc < 5) throw std::runtime_error("usage: ./main filename.dat tabulength maxiter init [readPos] [N]");

        // argc >= 4

        // argc == 4

        int tabuLength = atoi(argv[2]);
        int maxIter = atoi(argv[3]);
        int maxNonImprIter = 4 * tabuLength;  // 4 * (n/4) = n -> maxIter/2
        int timelimit = 420;                  // 7 minutes
        TSP tspInstance;
        int init = atoi(argv[4]);

        if (argc == 6) {
            tspInstance.readPos(argv[1]);  // it computes related costs
        } else if (argc > 6) {
            int N = atoi(argv[6]);
            tspInstance.setN(N);
            tspInstance.randomCosts(N);
        } else if (argc == 5) {
            tspInstance.readCostsFromFile(argv[1]);
        }

        TSPSolution initialSolution(tspInstance);

        CustomTimer t;  // start timer

        TSPSolver tspSolver;  // initialization
        if (init != 0)
            tspSolver.initHeuNearestNeighbour(tspInstance, initialSolution);
        else
            tspSolver.initRand(initialSolution);

        TSPSolution bestSolution(initialSolution);
        tspSolver.solve(tspInstance, initialSolution, tabuLength, maxIter, maxNonImprIter, t, timelimit, bestSolution);  /// solve with TSAC

        double micros = t.stopMicro();

        std::cout << "FROM: ";
        // initialSolution.print();
        std::cout << /*" - value : " <<*/ tspSolver.evaluate(initialSolution, tspInstance) << "\n";
        std::cout << "TO  : ";
        // bestSolution.print();
        std::cout /*" - value : " <<*/ << tspSolver.evaluate(bestSolution, tspInstance) << "\n";
        std::cout << "Time: " << micros * 1e-6 << " s\n";

    } catch (std::exception& e) {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    }
    return 0;
}
