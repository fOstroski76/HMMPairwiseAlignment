#include "utils.h"
#include "model.h"
#include <string>

using namespace std;

void symbol_transition(){

}

double get_emission(int x, int y, int state){
    return 0;
}

void copy(double* copy, double* to_copy, int size){
    for(int i = 0; i < size; i++){
        copy[i] = to_copy[i];
    }
}

int max_size(int seq_size){
    const int seq_len = seq_size;
    return min(MAX_SIZE, seq_len);
}

int conversion(char c){
    switch (c)
    {
    case 'A':
        return 0;
    case 'C':
        return 1;
    case 'G':
        return 2;
    case 'T':
        return 3;
    case '-':
        return 4;
    default:
        break;
    }
}
