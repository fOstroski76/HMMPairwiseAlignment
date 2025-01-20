#include "model.h"
#include "utils.h"

using namespace std;

//model parameters
double pis[3];
double trans[3][3];

// emission matrixes
double emission_M[4][4];
double emission_Ix[4];
double emission_Iy[4];

//alpha and beta
double alpha[MAX_SIZE][3];
double beta[MAX_SIZE][3];

// check if emission is possible
bool check_emission(char xt, char yt, int state){
    if(state == 0){
        if(xt == '-' || yt == '-') return false;
    } else {
        if(xt != '-' && yt != '-') return false;

        if(state == 1 && xt == '-') return false;

        if(state == 2 && yt == '-') return false;

    }   

    return true;
}

// get the emission depending on the state
double& get_emission(char xt, char yt, int state){
    if(state == 0){
        return emission_M[convert_char_into_int(xt)][convert_char_into_int(yt)];
    } else if(state == 1){
        return emission_Ix[convert_char_into_int(xt)];
    } else {
        return emission_Iy[convert_char_into_int(yt)];
    }
}

// forward algorithm for alpha
void forward_algorithm(pair<string, string> pairs){

    string x = pairs.first;
    string y = pairs.second;

    //init size and sum
    int size = max_size(x.size());
    double sum = 0.0;

    //init alpha values
    for(int i = 0; i < 3; i++){
        if(check_emission(x[0], y[0], i))
            sum += (alpha[0][i] = pis[i] * get_emission(x[0], y[0], i));
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
            
            if(check_emission(x[t], y[t], i))
                sum += (alpha[t][i] *= get_emission(x[t], y[t], i));
            else 
                sum += (alpha[t][i] *= 0.0);
        }

        //normalize
        for(int i = 0; i < 3; i++) alpha[t][i] /= sum;
    }
}

void backward_algorithm(pair<string, string> pairs){
    string x = pairs.first;
    string y = pairs.second;

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
    for(int t = size - 2; t >= 0; t--){
        sum = 0;

        //calc beta * emis in t
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(check_emission(x[t+1], y[t+1], j))
                    sum += (beta[t][i] += beta[t+1][j] * trans[i][j] * get_emission(x[t+1], y[t+1], j));
            }
        }

        //normalize
        for(int i = 0; i < 3; i++) beta[t][i] /= sum;
    }
}