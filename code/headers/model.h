#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <limits>
#include <utility>
#include <algorithm>

using namespace std;

const int MAX_SIZE = 20000;

// trans params
extern double pis[3];
extern double trans[3][3];

// emission arrays
extern double emission_M[4][4];
extern double emission_Ix[4];
extern double emission_Iy[4];

//alpha and beta
extern double alpha[MAX_SIZE][3];
extern double beta[MAX_SIZE][3];

// print model params
void print_model_params();
void print_alpha_beta();
void set_alpha_beta(int size);

// extra
double get_emission(char xt, char yt, int state);
bool check_emission(char xt, char yt, int state);

// evaluation functions
void forward_algorithm(pair<string, string> pair);
void backward_algorithm(pair<string, string> pair);

// learning function
void estimate_initial_prob(vector<pair<string, string>> dataset);
void baum_welch(int max_iterations, double tol, pair<string, string> dataset);

//deconding
void viterbi(pair<string, string> pair, const string& file_path);

#endif 