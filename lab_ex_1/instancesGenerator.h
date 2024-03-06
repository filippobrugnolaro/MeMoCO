#ifndef INSTANCES_GENERATOR_H
#define INSTANCES_GENERATOR_H

#include <unistd.h>

#include <fstream>
#include <random>
#include <set>

class instancesGenerator {
   private:
    std::set<std::pair<int, int>> holes;
    int maxSkip;

   protected:
    void generateHorizontalLine(int n, int xStart, int yStart, int length, int skip);
    void generateVerticalLine(int n, int xStart, int yStart, int length, int skip);
    void generateSquare(int n, int xStart, int yStart, int length, int skip);
    void generateTriangle(int remainingSize, int n, int xStart, int yStart, int height, int width, int skip);
    void generateTriangleSimple(int remainingSize, int n, int xStart, int yStart, int height, int width);
    void generateRectangle(int n, int xStart, int yStart, int height, int width, int skip);

   public:
    instancesGenerator();
    void writeInstancesToFile(const char* filename, int n);
    void generate(int n);
};
#endif