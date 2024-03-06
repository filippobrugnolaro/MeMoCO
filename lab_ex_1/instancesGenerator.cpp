#include "instancesGenerator.h"

#include <iostream>

instancesGenerator::instancesGenerator() : holes({}), maxSkip(1) {}

void instancesGenerator::generate(int n) {
    int remainingSize = n;  // 10% of the board
    int counter_triangles = 0;

    while (remainingSize > 0) {
        int lastSize = holes.size();

        std::random_device rd;
        std::mt19937_64 generator(rd());
        std::uniform_int_distribution<int> choice_distr(0, 4);

        int choice = choice_distr(generator);

        std::uniform_int_distribution<int> coord_distr(0, n);
        int xStart = coord_distr(generator);
        int yStart = coord_distr(generator);

        switch (choice) {
            case 0: {
                std::uniform_int_distribution<int> length_line_distr(1, n);
                int length = length_line_distr(generator);
                generateHorizontalLine(n, xStart, yStart, length, maxSkip);
                remainingSize -= (holes.size() - lastSize);
                break;
            }
            case 1: {
                std::uniform_int_distribution<int> length_line_distr(1, n);
                int length = length_line_distr(generator);
                generateVerticalLine(n, xStart, yStart, length, maxSkip);
                remainingSize -= (holes.size() - lastSize);
                break;
            }
            case 2: {
                std::uniform_int_distribution<int> length_square_distr(1, 7);
                int length = length_square_distr(generator);
                generateSquare(n, xStart, yStart, length, maxSkip);
                remainingSize -= (holes.size() - lastSize);
                break;
            }
            case 3: {
                int height;
                int width;

                std::uniform_int_distribution<int> triangle_choice_distr(0, 1);
                int rnd = triangle_choice_distr(generator);

                if (counter_triangles < 2 && rnd == 1) {
                    std::uniform_int_distribution<int> height_triangle_distr(1, 7);
                    height = height_triangle_distr(generator);
                    width = 2 * height - 1;
                    generateTriangle(remainingSize, n, xStart, yStart, height, width, maxSkip);
                    counter_triangles++;
                } else {
                    std::uniform_int_distribution<int> height_simple_triangle_distr(1, n);
                    height = height_simple_triangle_distr(generator);
                    width = 2 * height;
                    generateTriangleSimple(remainingSize, n, xStart, yStart, height, width);
                }
                remainingSize -= (holes.size() - lastSize);
                break;
            }
            case 4: {
                std::uniform_int_distribution<int> dims_rectangle_distr(1, 7);
                int width = dims_rectangle_distr(generator);
                int height = dims_rectangle_distr(generator);
                generateRectangle(n, xStart, yStart, width, height, maxSkip);
                remainingSize -= (holes.size() - lastSize);

                break;
            }
            default:
                break;
        }
    }
}

void instancesGenerator::writeInstancesToFile(const char* filename, int n) {
    generate(n);

    std::ofstream posFile(filename);

    posFile << n << std::endl;

    for (auto it = holes.begin(); it != holes.end(); it++) {
        posFile << it->first << " " << it->second << std::endl;
    }

    posFile.close();
}

void instancesGenerator::generateHorizontalLine(int n, int xStart, int yStart, int length, int skip) {
    int xEnd = xStart + (skip + 1) * length;
    if (xEnd < n) {
        if (length % 2 == 0) {
            if ((int)holes.size() == n) {
                return;
            }
            holes.insert({xStart, yStart});
            if ((int)holes.size() == n) {
                return;
            }
            holes.insert({xStart + length - 1, yStart});

        } else {
            for (int i = 0; i < length; i = i + skip + 1) {
                if ((int)holes.size() == n) {
                    return;
                }
                holes.insert({xStart + i, yStart});
            }
        }
    }
}

void instancesGenerator::generateVerticalLine(int n, int xStart, int yStart, int length, int skip) {
    int yEnd = yStart + (skip + 1) * length;
    if (yEnd < n) {
        if (length % 2 == 0) {
            if ((int)holes.size() == n) {
                return;
            }
            holes.insert({xStart, yStart});
            if ((int)holes.size() == n) {
                return;
            }
            holes.insert({xStart, yStart + length - 1});

        } else {
            for (int i = 0; i < length; i = i + skip + 1) {
                if ((int)holes.size() == n) {
                    return;
                }
                holes.insert({xStart, yStart + i});
            }
        }
    }
}

void instancesGenerator::generateSquare(int n, int xStart, int yStart, int length, int skip) {
    int xEnd = xStart + (skip + 1) * length;
    int yEnd = yStart + (skip + 1) * length;

    if (xEnd < n && yEnd < n) {
        if (length % 2 == 0) {
            if ((int)holes.size() == n) {
                return;
            }
            holes.insert({xStart, yStart});
            if ((int)holes.size() == n) {
                return;
            }
            holes.insert({xStart + length - 1, yStart});
            if ((int)holes.size() == n) {
                return;
            }
            holes.insert({xStart, yStart + length - 1});
            if ((int)holes.size() == n) {
                return;
            }
            holes.insert({xStart + length - 1, yStart + length - 1});

        } else {
            for (int i = 0; i < length; i = i + skip + 1) {
                for (int j = 0; j < length; j = j + skip + 1) {
                    if ((int)holes.size() == n) {
                        return;
                    }
                    holes.insert({xStart + j, yStart + i});
                }
            }
        }
    }
}

void instancesGenerator::generateTriangle(int remainingSize, int n, int xStart, int yStart, int height, int width, int skip) {
    int xEnd = xStart + (skip + 1) * width;
    int yEnd = yStart + (skip + 1) * height;

    if (width > 2 && height > 2) {
        if (xEnd < n && yEnd < n) {
            int spaces = width / 2;
            int stars = 1;
            bool isBaseEven = width % 2 == 0;

            if (isBaseEven) {
                spaces--;
                stars++;
                skip = 0;
            }
            for (int i = height - 1; i >= 0; i--) {
                int k = spaces;
                for (; k < spaces; k++) {
                    continue;
                }

                for (int j = stars - 1; j >= 0; j--) {
                    if ((int)holes.size() == n) {
                        return;
                    }
                    holes.insert(std::make_pair(xStart + k + j, yStart + i));
                    for (int k = 0; k < skip; k++) {
                        j -= skip;
                    }
                }

                if (i == height - 1 && isBaseEven) {
                    continue;
                } else {
                    spaces--;
                    stars += 2;
                }
            }
        }
    }
}

void instancesGenerator::generateTriangleSimple(int remainingSize, int n, int xStart, int yStart, int height, int width) {
    int xEnd = xStart + width;
    int yEnd = yStart + height;

    if (width > 2 && height > 2) {
        if (xEnd < n && yEnd < n) {
            if ((int)holes.size() == n) {
                return;
            }
            holes.insert({xStart, yStart});
            if ((int)holes.size() == n) {
                return;
            }
            holes.insert({xStart + width, yStart});
            if ((int)holes.size() == n) {
                return;
            }
            int median = (xStart + xEnd) / 2;
            holes.insert({median, yStart + height});
        }
    }
}

void instancesGenerator::generateRectangle(int n, int xStart, int yStart, int width, int height, int skip) {
    int xEnd = xStart + (skip + 1) * width;
    int yEnd = yStart + (skip + 1) * height;

    if (width > 3 && height > 3) {
        if (xEnd < n && yEnd < n) {
            if (width % 2 == 0 || height % 2 == 0) {
                if ((int)holes.size() == n) {
                    return;
                }
                holes.insert({xStart, yStart});
                if ((int)holes.size() == n) {
                    return;
                }
                holes.insert({xStart + width - 1, yStart});
                if ((int)holes.size() == n) {
                    return;
                }
                holes.insert({xStart, yStart + height - 1});
                if ((int)holes.size() == n) {
                    return;
                }
                holes.insert({xStart + width - 1, yStart + height - 1});
            } else {
                if (width != height) {
                    for (int i = 0; i < height; i = i + skip + 1) {
                        for (int j = 0; j < width; j = j + skip + 1) {
                            if ((int)holes.size() == n) {
                                return;
                            }
                            holes.insert({xStart + i, yStart + j});
                        }
                    }
                }
            }
        }
    }
}