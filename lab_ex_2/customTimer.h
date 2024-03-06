#ifndef CUSTOM_TIMER_H
#define CUSTOM_TIMER_H

#include <chrono>

class CustomTimer {
   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startpoint;

   public:
    CustomTimer();
    double getStartPoint();
    double stopMicro();
    double stopMilli();
};

#endif