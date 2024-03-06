#include "customTimer.h"

CustomTimer::CustomTimer() : startpoint(std::chrono::high_resolution_clock::now()) {}

/*
    Calculate the time passed since the startpoint in microseconds
*/
double CustomTimer::stopMicro() {
    auto endpoint = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startpoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endpoint).time_since_epoch().count();
    auto duration = end - start;
    return duration;
}

/*
    Calculate the time passed since the startpoint in milliseconds
*/
double CustomTimer::stopMilli() {
    auto endpoint = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(startpoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(endpoint).time_since_epoch().count();
    auto duration = end - start;
    return duration;
}

/*
    Get the startpoint in milliseconds
*/
double CustomTimer::getStartPoint() {
    return std::chrono::time_point_cast<std::chrono::milliseconds>(startpoint).time_since_epoch().count();
}