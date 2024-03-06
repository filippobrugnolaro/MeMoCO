#include "TSP.h"

TSP::TSP() : n(0), cost({}), infinite(1e10) {}

int TSP::getN() const {
    return this->n;
}

std::vector<std::vector<double>> TSP::getCost() const {
    return this->cost;
}

double TSP::getInfinite() const {
    return this->infinite;
}

void TSP::setN(int n) {
    this->n = n;
}

void TSP::setInfinite() {  // set infinite value
    infinite = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            infinite += cost[i][j];
        }
    }
    infinite *= 2;
}

void TSP::readCostsFromFile(const char* filename)  // read cost matrix from file
{
    std::ifstream in(filename);

    // read size
    in >> n;

    // read costs
    cost.resize(n);
    for (int i = 0; i < n; i++) {
        cost[i].reserve(n);
        for (int j = 0; j < n; j++) {
            double c;
            in >> c;
            cost[i].push_back(c);
        }
    }
    in.close();

    setInfinite();
}

void TSP::readPos(const char* filename)  // read positions from file and compute related costs
{
    std::ifstream in(filename);

    // read size
    in >> n;

    // read positions
    std::vector<std::vector<double>> pos;
    pos.resize(n);
    for (int i = 0; i < n; i++) {
        pos[i].reserve(2);
        for (int j = 0; j < 2; j++) {
            double c;
            in >> c;
            pos[i].push_back(c);
        }
    }
    in.close();

    // compute costs
    computeCost(pos);
}

void TSP::computeCost(const std::vector<std::vector<double>>& pos)  // compute costs from positions
{
    cost.resize(n);
    for (int i = 0; i < n; i++) {
        cost[i].reserve(n);
        for (int j = 0; j < n; j++) {
            double c = abs(pos[i][0] - pos[j][0]) + abs(pos[i][1] - pos[j][1]);  // Manhatan distance: abs(x_i - x_j) + abs(y_i - y_j)
            cost[i].push_back(c);
        }
    }
    setInfinite();
}

void TSP::randomCosts(const int n)  // random positions generations
{
    std::vector<std::vector<double>> allPos;
    std::vector<std::vector<double>> pos;

    // Random from 0 to N-1
    allPos.resize(n * n);
    int max = n;
    int min = 0;

    // Nested loop for all possible pairs
    int a = 0;
    for (int i = min; i < max; i++) {
        for (int j = min; j < max; j++) {
            allPos[a].resize(2);
            allPos[a][0] = i;
            allPos[a][1] = j;
            a++;
        }
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(allPos.begin(), allPos.end(), generator);  // shuffle all pairs

    pos.resize(n);
    for (int i = 0; i < n; i++) {
        pos[i].resize(2);
        pos[i][0] = allPos[i][0];
        pos[i][1] = allPos[i][1];  // save postions as the first N from all the pairs
    }

    // compute costs
    computeCost(pos);
}