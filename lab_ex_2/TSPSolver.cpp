/**
 * @file TSPSolver.cpp
 * @brief TSP solver (neighborhood search)
 *
 */

#include "TSPSolver.h"

#include <iostream>

#include "TSPMove.h"

TSPSolver::TSPSolver() {}

void TSPSolver::initTabuList(int n) {
    for (int i = 0; i < n; ++i)
        tabuList.push_back(-tabuLength - 1);
    // at iterarion 0, no neighbor is tabu --> iteration(= 0) - tabulistInit > tabulength --> tabulistInit < tabuLength + 0
}

void TSPSolver::updateTabuList(int nodeFrom, int nodeTo, int iter) {
    tabuList[nodeFrom] = iter;
    tabuList[nodeTo] = iter;
}

bool TSPSolver::isTabu(int nodeFrom, int nodeTo, int iter) {
    return ((iter - tabuList[nodeFrom] <= tabuLength) && (iter - tabuList[nodeTo] <= tabuLength));
}

double TSPSolver::evaluate(const TSPSolution& sol, const TSP& tsp) const {
    double total = 0.0;
    for (int i = 0; i < (int)sol.getSequence().size() - 1; ++i) {
        int from = sol.getSequence()[i];
        int to = sol.getSequence()[i + 1];
        total += tsp.getCost()[from][to];
    }
    return total;
}

bool TSPSolver::initRand(TSPSolution& sol) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> idx_distr(1, sol.getSequence().size() - 2);

    for (int i = 1; i < (int)sol.getSequence().size(); ++i) {
        // intial and final position are fixed (initial/final node remains 0)
        int idx1 = idx_distr(generator);
        int idx2 = idx_distr(generator);
        int tmp = sol.getSequence()[idx1];
        sol.setSequence(idx1, sol.getSequence()[idx2]);
        sol.setSequence(idx2, tmp);
    }

    return true;
}

// heuristic initial solution -> choose min from each row
bool TSPSolver::initHeuNearestNeighbour(const TSP& tsp, TSPSolution& sol) {
    // clean sequence [0, -1, ..., -1, 0]
    sol.setSequence(0, 0);
    sol.setSequence(sol.getSequence().size() - 1, 0);
    for (int i = 1; i < (int)sol.getSequence().size() - 1; i++) {
        sol.setSequence(i, -1);
    }

    std::vector<int> visited;
    int i = 0;
    visited.push_back(0);
    int prev = 0;

    while (i < tsp.getN() - 1) {  // [0, (n-1 iterations), 0]
        // sort as indices
        std::vector<int> indices(tsp.getN());
        std::iota(indices.begin(), indices.end(), 0);  // Initializing 0,1,2,3,...,n-1
        sort(indices.begin(), indices.end(), [&](int a, int b) { return tsp.getCost()[prev][a] < tsp.getCost()[prev][b]; });

        bool explored = false;
        for (int j = 1; j < (int)indices.size() && !explored; j++) {  // exclude actual min since it is autoloop
            int temp = indices[j];
            if (!std::count(visited.begin(), visited.end(), temp)) {
                sol.setSequence(i + 1, temp);
                visited.push_back(temp);
                prev = temp;
                i++;
                explored = true;
            }
        }
    }

    return true;
}

bool TSPSolver::solve(const TSP& tsp, const TSPSolution& initSol, int tabulength, int maxIter, int maxNonImprIter, CustomTimer& timer, int timeLimit, TSPSolution& bestSol)  /// TS: new param
{
    try {
        bool stop = false;
        int currentGlobalIter = 0;
        // int currentNonImprIter = 0;

        /// Tabu Search
        this->tabuLength = tabulength;
        tabuList.reserve(tsp.getN());
        initTabuList(tsp.getN());

        TSPSolution currSol(initSol);
        double bestValue, currValue;
        bestValue = currValue = evaluate(currSol, tsp);
        TSPMove move;

        while (!stop) {
            ++currentGlobalIter;
            // std::cout << currentGlobalIter << " - Val: " << bestValue << std::endl;
            double aspiration = bestValue - currValue;
            double bestNeighValue = currValue + findBestNeighbor(tsp, currSol, currentGlobalIter, aspiration, move);

            if (bestNeighValue >= tsp.getInfinite()) {  /// stop because all neighbours are tabu
                // std::cout << "ALL NEIGHBOURS TABU" << std::endl;
                stop = true;
                continue;
            }

            updateTabuList(currSol.getSequence()[move.getFrom()], currSol.getSequence()[move.getTo()], currentGlobalIter);  /// insert move info into tabu list

            currSol = swap(currSol, move);
            currValue = bestNeighValue;
            if (currValue < bestValue - 0.01) {  // update best solution if it has improved
                bestValue = currValue;
                bestSol = currSol;
                // currentNonImprIter = 0;  /// ONLY FOR MAX NON-IMPROVING IT: reset non-improving iterations since it has just improved
            }  // else {
            //    currentNonImprIter++;
            //    std::cout << "Non-improving iter: " << currentNonImprIter << std::endl;
            // }

            bool maxIterReached = currentGlobalIter == maxIter;
            // bool nonImprIterReached = currentNonImprIter == maxNonImprIter;
            // bool timeLimitReached = timer.stopMilli() * 1e-3 > timeLimit;

            if (maxIterReached) {  /// stop criteria
                stop = true;
            }
        }

    } catch (std::exception& e) {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
        return false;
    }
    return true;
}

TSPSolution& TSPSolver::swap(TSPSolution& tspSol, const TSPMove& move) {
    TSPSolution tmpSol(tspSol);
    for (int i = move.getFrom(); i <= move.getTo(); ++i) {
        tspSol.setSequence(i, tmpSol.getSequence()[move.getTo() - (i - move.getFrom())]);
    }
    return tspSol;
}

/* Determine the NON-TABU *move* yielding the best 2-opt neighbor solution
 * Aspiration criteria: 'neighCostVariation' better than 'aspiration' (notice that 'aspiration'
 * has been set such that if 'neighCostVariation' is better than 'aspiration' than we have a
 * new incumbent solution)
 */
double TSPSolver::findBestNeighbor(const TSP& tsp, const TSPSolution& currSol, int currIter, double aspiration, TSPMove& move) {
    double bestCostVariation = tsp.getInfinite();

    // intial and final position are fixed (initial/final node remains 0)
    for (int a = 1; a < (int)currSol.getSequence().size() - 2; a++) {
        int h = currSol.getSequence()[a - 1];
        int i = currSol.getSequence()[a];

        for (int b = a + 1; b < (int)currSol.getSequence().size() - 1; b++) {
            int j = currSol.getSequence()[b];
            int l = currSol.getSequence()[b + 1];

            double neighCostVariation = -tsp.getCost()[h][i] - tsp.getCost()[j][l] + tsp.getCost()[h][j] + tsp.getCost()[i][l];

            if (isTabu(i, j, currIter) && !(neighCostVariation < aspiration - 0.01)) {
                continue;  // check if tabu and not aspiration criteria
            }
            if (neighCostVariation < bestCostVariation) {
                bestCostVariation = neighCostVariation;
                move.setFrom(a);
                move.setTo(b);
            }
        }
    }
    return bestCostVariation;
}
