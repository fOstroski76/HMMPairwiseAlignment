#include "../headers/model.h"
#include "../headers/utils.h"

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

// setting inital values for each train
void set_alpha_beta(int size){
    for(int t = 0; t < size; t++){
        for(int i = 0; i < 3; i++){
            alpha[t][t] = 0.0;
            beta[t][i] = 0.0;
        }
    }
}

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
double get_emission(char xt, char yt, int state){
    if(!check_emission(xt, yt, state))  return -numeric_limits<double>::infinity();

    if(state == 0){
        return emission_M[convert_char_into_int(xt)][convert_char_into_int(yt)];
    } else if(state == 1){
        return emission_Ix[convert_char_into_int(xt)];
    } else {
        return emission_Iy[convert_char_into_int(yt)];
    }
}

/**
 * @brief Function for calculating forward probabilities.
 * 
 * This function takes a pair of sequences and calculates the forward probabilities 
 * for the given pair. The probabilities are stored in the alpha double array.
 * 
 * @param pairs The pair of strings using the std::pair class from the C++ Standard Library.
 * 
 * @note The backward probabilities are stored as log form.
 * @warning The sequences in the pair provided to this algorithm must be of the same length.
 */
void forward_algorithm(pair<string, string> pairs){
    string x = pairs.first;
    string y = pairs.second;

    //init size and sum
    int size = max_size(x.length());
    vector<double> log_adds;

    //init alpha values
    for(int i = 0; i < 3; i++){
        alpha[0][i] = pis[i] + get_emission(x[0], y[0], i);
    }

    //iterate over alpha for each t
    for(int t = 1; t < size; t++){
        // calc alpha in t
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                log_adds.push_back(alpha[t-1][j] + trans[j][i] + get_emission(x[t], y[t], i));
            }

            alpha[t][i] = log_sum_exp(log_adds);
            log_adds.clear();
        }
    }
}

/**
 * @brief Function for calculating backward probabilities.
 * 
 * This function takes a pair of sequences and calculates the backward probabilities 
 * for the given pair. The probabilities are stored in the beta double array.
 * 
 * @param pairs The pair of strings using the std::pair class from the C++ Standard Library.
 * 
 * @note The backward probabilities are stored as log form.
 * @warning The sequences in the pair provided to this algorithm must be of the same length.
 */
void backward_algorithm(pair<string, string> pairs){
    string x = pairs.first;
    string y = pairs.second;

    //init size and sum
    int size = max_size(x.length());
    vector<double> log_adds;

    //init beta in T-1 time
    for(int i = 0; i < 3; i++){
        beta[size - 1][i] = 0.0;//scaler[size-1];
    }

    //iterate from T-1 to 0 for each beta
    for(int t = size - 2; t >= 0; t--){

        //calc beta * emis in t
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                log_adds.push_back(beta[t+1][j] + trans[i][j] + get_emission(x[t+1], y[t+1], j));

            }
            
            beta[t][i] = log_sum_exp(log_adds);
            log_adds.clear();
        }  
    }
}