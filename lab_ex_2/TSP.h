#ifndef TSP_H
#define TSP_H

#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

/**
 * Class that describes a TSP instance (a cost matrix, nodes are identified by integer 0 ... n-1)
 */
class TSP {
   private:
    int n;  // number of nodes
    std::vector<std::vector<double>> cost;
    double infinite;  // infinite value (an upper bound on the value of any feasible solution)

   public:
    TSP();
    int getN() const;
    std::vector<std::vector<double>> getCost() const;
    double getInfinite() const;

    void setN(int n);
    void setInfinite();

    void readCostsFromFile(const char* filename);
    void readPos(const char* filename);
    void computeCost(const std::vector<std::vector<double>>& pos);
    void randomCosts(const int N);
};

#endif