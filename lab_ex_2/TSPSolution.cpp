#include "TSPSolution.h"

TSPSolution::TSPSolution(const TSP& tsp) {
    sequence.reserve(tsp.getN() + 1);
    for (int i = 0; i < tsp.getN(); ++i) {
        sequence.push_back(i);
    }
    sequence.push_back(0);
}

TSPSolution::TSPSolution(const TSPSolution& tspSol) {
    sequence.reserve(tspSol.getSequence().size());
    for (int i = 0; i < (int)tspSol.getSequence().size(); ++i) {
        sequence.push_back(tspSol.getSequence()[i]);
    }
}

std::vector<int> TSPSolution::getSequence() const {
    return sequence;
}

void TSPSolution::setSequence(int pos, int val) {
    sequence[pos] = val;
}

void TSPSolution::print() {
    for (int i = 0; i < (int)sequence.size(); i++) {
        std::cout << sequence[i] << " ";
    }
}

TSPSolution& TSPSolution::operator=(const TSPSolution& right) {
    if (this == &right) return *this;
    for (int i = 0; i < (int)sequence.size(); i++) {
        sequence[i] = right.sequence[i];
    }
    return *this;
}