#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include <unistd.h>

#include "TSPMove.h"
#include "TSPSolution.h"
#include "customTimer.h"

/**
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class TSPSolver {
   private:
    int tabuLength;
    std::vector<int> tabuList;

   protected:
    double findBestNeighbor(const TSP& tsp, const TSPSolution& currSol, int currIter, double aspiration, TSPMove& move);
    TSPSolution& swap(TSPSolution& tspSol, const TSPMove& move);
    void initTabuList(int n);
    void updateTabuList(int nodeFrom, int nodeTo, int iter);
    bool isTabu(int nodeFrom, int nodeTo, int iter);

   public:
    TSPSolver();
    double evaluate(const TSPSolution& sol, const TSP& tsp) const;
    bool initRand(TSPSolution& sol);
    bool initHeuNearestNeighbour(const TSP& tsp, TSPSolution& sol);
    bool solve(const TSP& tsp, const TSPSolution& initSol, int tabulength, int maxIter, int maxNonImprIter, CustomTimer& timer, int timeLimit, TSPSolution& bestSol);
};

#endif