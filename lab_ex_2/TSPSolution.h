#ifndef TSP_SOLUTION_H
#define TSP_SOLUTION_H

#include "TSP.h"

/**
 * TSP Solution representation: ordered sequence of nodes (path representation)
 */
class TSPSolution {
   private:
    std::vector<int> sequence;

   public:
    /** Constructor
     * build a standard solution as the sequence <0, 1, 2, 3 ... n-1, 0>
     * @param tsp TSP instance
     * @return ---
     */
    TSPSolution(const TSP& tsp);

    /** Copy constructor
     * build a solution from another
     * @param tspSol TSP solution
     * @return ---
     */
    TSPSolution(const TSPSolution& tspSol);

    /** getSequence method
     * @param ---
     * @return sequence of nodes
     */
    std::vector<int> getSequence() const;

    void setSequence(int pos, int val);

    /** print method
     * @param ---
     * @return ---
     */
    void print();

    /** assignment method
     * copy a solution into another one
     * @param right TSP solution to get into
     * @return ---
     */
    TSPSolution& operator=(const TSPSolution& right);
};

#endif
