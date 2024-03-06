/**
 * @file main.cpp
 * @brief initial model test for 10 holes with layout similar to the picture
 */

#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "cpxmacro.h"
#include "customTimer.h"
#include "instancesGenerator.h"

using namespace std;

// error status and messagge buffer
int status;
char errmsg[BUF_SIZE];

const int NAME_SIZE = 512;
char name[NAME_SIZE];

/*
    Setup the LP problem
*/
void setupLP(CEnv env, Prob lp, const int N, const std::vector<std::vector<double>>& C) {
    std::vector<int> nameN(N);                         // vector with N ints
    std::iota(std::begin(nameN), std::end(nameN), 0);  // Fill with 0, 1, ..., N-1

    // counter for mapping variables
    int current_var_position = 0;

    // mapping for reading the correct position of the variables xs
    vector<vector<int>> map_x;
    map_x.resize(N);
    for (int i = 0; i < N; ++i) {
        map_x[i].resize(N);
        for (int j = 0; j < N; ++j) {
            map_x[i][j] = -1;
        }
    }

    // add x vars [not in o.f.]
    for (int i = 0; i < N; i++) {
        for (int j = 1; j < N; j++)  // no need for x_i0 forall i
        {
            if (i == j) continue;  // no need for x_ii
            char xtype = 'C';
            double lb = 0.0;
            double ub = CPX_INFBOUND;
            snprintf(name, NAME_SIZE, "x_%d_%d", nameN[i], nameN[j]);
            char* xname = (char*)(&name[0]);
            double obj = 0.0;
            CHECKED_CPX_CALL(CPXnewcols, env, lp, 1, &obj, &lb, &ub, &xtype, &xname);  // no need for obj since not in o.f.
            map_x[i][j] = current_var_position++;
        }
    }

    // mapping for reading the correct position of the variables ys
    vector<vector<int>> map_y;
    map_y.resize(N);
    for (int i = 0; i < N; ++i) {
        map_y[i].resize(N);
        for (int j = 0; j < N; ++j) {
            map_y[i][j] = -1;
        }
    }

    // add y vars [in o.f.: sum{ij} C_ij y_ij + ... ]
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) continue;  // no need for y_ii
            char xtype = 'B';
            double lb = 0.0;
            double ub = 1.0;
            snprintf(name, NAME_SIZE, "y_%d_%d", nameN[i], nameN[j]);
            char* xname = (char*)(&name[0]);
            CHECKED_CPX_CALL(CPXnewcols, env, lp, 1, &C[i][j], &lb, &ub, &xtype, &xname);
            map_y[i][j] = current_var_position++;
        }
    }

    // add constraints [ forall k in N, k != 0, sum(i,k) x_ik - sum(k,j) x_kj = 1, j!=0]
    for (int k = 1; k < N; k++) {
        std::vector<int> idx;
        std::vector<double> coef;
        char sense = 'E';
        // sum of incomings
        for (int i = 0; i < N; i++) {
            if (i == k) continue;        // exclude x_kk
            idx.push_back(map_x[i][k]);  // corresponds to variable x_ik
            coef.push_back(1.0);
        }
        // sum of outcomings, doing subtraction
        for (int j = 1; j < N; j++) {
            if (j == k) continue;        // exclude x_kk
            idx.push_back(map_x[k][j]);  // corresponds to the variable x_kj
            coef.push_back(-1.0);        // subtracted
        }
        int matbeg = 0;
        double rhs = 1;
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
    }

    // add constraints [ forall i, sum{j} y_ij = 1 ]
    for (int i = 0; i < N; i++) {
        std::vector<int> idx;
        std::vector<double> coef;
        char sense = 'E';
        for (int j = 0; j < N; j++) {
            if (map_y[i][j] == -1) continue;  // exclude y_ii
            idx.push_back(map_y[i][j]);       // corresponds to the variable y_ij
            coef.push_back(1.0);
        }
        int matbeg = 0;
        double rhs = 1;
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
    }

    // add constraints [ forall j, sum{i} y_ij = 1 ]
    for (int j = 0; j < N; j++) {
        std::vector<int> idx;
        std::vector<double> coef;
        char sense = 'E';
        for (int i = 0; i < N; i++) {
            if (map_y[i][j] == -1) continue;  // exclude y_ii
            idx.push_back(map_y[i][j]);       // corresponds to the variable y_ij
            coef.push_back(1.0);
        }
        int matbeg = 0;
        double rhs = 1;
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
    }

    // add constraints [ forall (i,j) in A, j != 0, x_ij <= (N-1) y_ij ]
    // add constraints [ forall (i,j) in A, j != 0, x_ij - (N-1) y_ij <= 0, but
    // -> we change (y_ij) coeff obtaining this. -> x_ij + (1-N) y_ij <= 0 ]
    for (int i = 0; i < N; i++) {
        for (int j = 1; j < N; j++) {
            if (map_x[i][j] == -1) continue;  // exclude x_ii
            std::vector<int> idx = {map_x[i][j], map_y[i][j]};
            std::vector<double> coef = {1.0, 1.0 - N};
            char sense = 'L';
            int matbeg = 0;
            double rhs = 0;
            CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
        }
    }
}

/*
    Read size from file
*/
int readSizeFromFile(const char* filename) {
    // open file
    std::ifstream in(filename);

    // read size
    int n = 0;
    in >> n;

    // close file
    in.close();

    return n;
}

/*
    Read positions from file
*/
void readPos(const char* filename, std::vector<std::vector<double>>& pos) {
    std::ifstream in(filename);

    // read size
    int n = 0;
    in >> n;

    // read positions
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
}

/*
    Write positions to file
*/
void writePosToFile(const int n, std::vector<std::vector<double>>& pos, const char* filename) {
    // create and open
    ofstream posFile(filename);

    // save size
    posFile << n << "\n";

    // save positions
    for (auto it = pos.begin(); it != pos.end(); it++) {
        posFile << (*it)[0] << " " << (*it)[1] << std::endl;
    }

    posFile.close();
}

/*
    Read cost matrix from file
*/
void readCostsFromFile(std::vector<std::vector<double>>& cost, const char* filename) {
    // open file
    std::ifstream in(filename);

    // read size
    int n = 0;
    in >> n;

    // read costs
    double c;
    cost.resize(n);
    for (int i = 0; i < n; i++) {
        cost[i].reserve(n);
        for (int j = 0; j < n; j++) {
            in >> c;
            cost[i].push_back(c);
        }
    }

    // close file
    in.close();
}

// write cost matrix to file
void writeCostsToFile(const int n, std::vector<std::vector<double>>& cost, const char* filename) {
    // create and open
    ofstream matrixCostsFile(filename);

    // save size
    matrixCostsFile << n << "\n";

    // save costs
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            matrixCostsFile << cost[i][j] << "\t\t";
        }
        matrixCostsFile << cost[i][n - 1] << "\n";  // last element of row
    }

    matrixCostsFile.close();
}

/*
    Compute costs of the positions matrix
*/
void computeCosts(const int n, const std::vector<std::vector<double>>& pos, std::vector<std::vector<double>>& cost) {
    // compute costs
    cost.resize(n);
    for (int i = 0; i < n; i++) {
        cost[i].reserve(n);
        for (int j = 0; j < n; j++) {
            double c = abs(pos[i][0] - pos[j][0]) + abs(pos[i][1] - pos[j][1]);  // Manhatan distance: abs(x_i - x_j) + abs(y_i - y_j)
            cost[i].push_back(c);
        }
    }
}

/*
    Generate random positions
*/
void generateRandomPos(const int n, std::vector<std::vector<double>>& pos) {
    std::vector<std::vector<double>> allPos;

    allPos.resize(n * n);
    int max = n;
    int min = 0;

    // All possible positions
    int a = 0;
    for (int i = min; i < max; i++) {
        for (int j = min; j < max; j++) {
            allPos[a].resize(2);
            allPos[a][0] = i;
            allPos[a][1] = j;
            a++;
        }
    }

    // initialize generator
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::shuffle(allPos.begin(), allPos.end(), generator);  // shuffle all pairs

    // save the first N positions
    pos.resize(n);
    for (int i = 0; i < n; i++) {
        pos[i].resize(2);
        pos[i][0] = allPos[i][0];
        pos[i][1] = allPos[i][1];
    }
}

int main(int argc, char const** argv) {
    try {
        /*
            ./main savedPosFile.dat savedCostsFile.dat [N] [ReadCosts] [Random]

            savedPosFile.dat:
            - pos.dat:      argc == 4, argc == 6
            - not used:     argc == 5

            savedCostsFile.dat:
            - savedCosts.dat: argc == 4, argc == 5, argc == 6

            N: Number of nodes

            readCosts:
            - readCosts: argc == 5

            TypeOfInstance:
            random: argc == 6
        */

        if (argc < 4 || argc > 6) {
            throw std::runtime_error("usage: ./main filename.dat savedCostsFile.dat N [ReadCosts] [TypeOfInstance]");
        }
        std::vector<std::vector<double>> cost;
        std::vector<std::vector<double>> pos;
        int N = 0;

        if (argc == 6) {  // creation of new instance randomly, e.g. ./main savedPosFile.dat savedCostsFile.dat N x random
            N = atoi(argv[3]);
            generateRandomPos(N, pos);
            computeCosts(N, pos, cost);
            writePosToFile(N, pos, argv[1]);
            writeCostsToFile(N, cost, argv[2]);
        } else if (argc == 5) {  // read from existing instance of costs, e.g. ./main x.dat savedCostsFile.dat x readCosts
            N = readSizeFromFile(argv[2]);
            readCostsFromFile(cost, argv[2]);
        } else {  // creation of new instance with geometric figures, e.g. ./main savedPosFile.dat savedCostsFile.dat N
            instancesGenerator generator;
            N = atoi(argv[3]);
            generator.writeInstancesToFile(argv[1], N);
            readPos(argv[1], pos);
            computeCosts(N, pos, cost);
            writeCostsToFile(N, cost, argv[2]);
        }

        // init
        DECL_ENV(env);
        DECL_PROB(env, lp);

        // setup LP
        setupLP(env, lp, N, cost);
        CHECKED_CPX_CALL(CPXsetdblparam, env, CPX_PARAM_TILIM, 2700);

        // print problem
        CHECKED_CPX_CALL(CPXwriteprob, env, lp, "test/tsp.lp", NULL);

        // optimize LP
        CustomTimer t;
        CHECKED_CPX_CALL(CPXmipopt, env, lp);
        std::cout << "Time: " << t.stopMicro() * 1e-6 << " s" << std::endl;

        // print solution
        double objval;
        CHECKED_CPX_CALL(CPXgetobjval, env, lp, &objval);
        std::cout << "Objval: " << objval << std::endl;

        CHECKED_CPX_CALL(CPXsolwrite, env, lp, "test/tsp.sol");

        // free
        CPXfreeprob(env, &lp);
        CPXcloseCPLEX(&env);

    } catch (std::exception& e) {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    }
    return 0;
}
