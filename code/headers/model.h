#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <limits>

using namespace std;

const int MAX_SIZE = 10000;

extern double pis[3];
extern double trans[3][3];
extern double emissions[3][5][5];

//alpha and beta
extern double alpha[MAX_SIZE][3];
extern double beta[MAX_SIZE][3];

// evaluation functions
void forward_algorithm(vector<int> x, vector<int> y);
void backward_algorithm(vector<int> x, vector<int> y);

#endif 