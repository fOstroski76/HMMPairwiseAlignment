#ifndef UTILS_H
#define UTILS_H

#include <vector>

using namespace std;

double get_emission(int x, int y, int state);

void copy(double* copy, double* to_copy, int size);

int max_size(int seq_size);

int conversion(char c);

#endif