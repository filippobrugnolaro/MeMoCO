#include "TSPMove.h"

TSPMove::TSPMove() {}

int TSPMove::getFrom() const {
    return from;
}

int TSPMove::getTo() const {
    return to;
}

void TSPMove::setFrom(int from) {
    this->from = from;
}

void TSPMove::setTo(int to) {
    this->to = to;
}