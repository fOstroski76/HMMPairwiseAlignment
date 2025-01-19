#include "model.h"
#include "utils.h"

#include <vector>

using namespace std;

//model parameters
double pis[3];
double trans[3][3];
double emissions[3][5][5];

//alpha and beta
double alpha[MAX_SIZE][3];
double beta[MAX_SIZE][3];

void forward_algorithm(vector<int> x, vector<int> y){

    //init size and sum
    int size = max_size(x.size());
    double sum = 0;

    //init alpha values
    for(int i = 0; i < 3; i++){
        sum += (alpha[0][i] = pis[i] * emissions[i][x[0]][y[0]]);
    }

    //normalize starting values
    for(int i = 0; i < 3; i++){
        alpha[0][i] /= sum;
    }

    //iterate over alpha for each t
    for(int t = 1; t < size; t++){
        sum = 0;
        
        // calc alpha in t
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                alpha[t][i] += alpha[t-1][j] * trans[j][i];
            }
            
            sum += (alpha[t][i] *= emissions[i][x[t]][y[t]]);
        }

        //normalize
        for(int i = 0; i < 3; i++) alpha[t][i] /= sum;
    }
}

void backward_algorithm(vector<int> x, vector<int> y){

    //init size and sum
    int size = max_size(x.size());
    double sum = 0;

    //init beta in T-1 time
    for(int i = 0; i < 3; i++){
        sum += (beta[size - 1][i] = 1);
    }

    //normalize
    for(int i = 0; i < 3; i++){
        beta[size - 1][i] = sum;
    }

    //iterate from T-1 to 0 for each beta
    for(int t = x.size() - 2; t >= 0; t--){
        sum = 0;

        //calc beta * emis in t
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                beta[t][i] += beta[t+1][j] * trans[i][j] * emissions[j][x[t+1]][y[t+1]];
            }
            
            sum += (beta[t][i] *= emissions[i][x[t]][y[t]]);
        }

        //normalize
        for(int i = 0; i < 3; i++) beta[t][i] /= sum;
    }
}