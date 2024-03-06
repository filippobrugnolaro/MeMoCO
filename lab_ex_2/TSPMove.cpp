#include "TSPMove.h"

TSPMove::TSPMove() {}

/*
    Get the from node
*/
int TSPMove::getFrom() const {
    return from;
}

/*
    Get the to node
*/
int TSPMove::getTo() const {
    return to;
}

/*
    Set the from node
*/
void TSPMove::setFrom(int from) {
    this->from = from;
}

/*
    Set the to node
*/
void TSPMove::setTo(int to) {
    this->to = to;
}