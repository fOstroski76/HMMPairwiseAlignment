#include "../headers/model.h"
#include "../headers/utils.h"

using namespace std;

//gammas
double gamma[MAX_SIZE][3];
double sum_gamma[3];

//emission gammas
double gamma_M[4][4];
double gamma_Ix[4];
double gamma_Iy[4];

//xis
double xi[3][3];

// functions for printing
void print_xi(){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            cout << xi[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void print_gamma(){
    for(int i = 0; i < 10; i++){
        for(int t = 0; t < 3; t++){
            cout << gamma[i][t] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void print_estimate_gammas(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cout << gamma_M[i][j] << " ";
        }
        cout << endl;
    }   
    cout << endl;

    for(int i = 0; i < 4; i++){
        cout << gamma_Ix[i] << " ";
    }
    cout << endl << endl;

    for(int i = 0; i < 4; i++){
        cout << gamma_Iy[i] << " ";
    }
    cout << endl << endl;
}

//reset xi and sum_xi for every iteration and every value
void reset_xi_and_gamma(){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            xi[i][j] = 0.0;
        }
    }

    for(int t = 0; t < MAX_SIZE; t++){
        gamma[t][0] = 0.0;
        gamma[t][1] = 0.0;
        gamma[t][2] = 0.0;
    }

    sum_gamma[0] = 0.0;
    sum_gamma[1] = 0.0;
    sum_gamma[2] = 0.0;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            gamma_M[i][j] = 0.0;
        }
        gamma_Ix[i] = 0.0;
        gamma_Iy[i] = 0.0;
    }
}

//function for calculation gamma for each step t
void calc_gamma(int t, int size){
    vector<double> log_adds;

    // calc gamma in step t
    for(int i = 0; i < 3; i++){
        log_adds.push_back(alpha[t][i] + beta[t][i]);
    }

    double sum = log_sum_exp(log_adds);
    for(int i = 0; i < 3; i++)
        gamma[t][i] = log_adds[i] - sum;
}

//function for calculationg xi in each step t
void calc_xi(int t, char x, char y){
    vector<double> log_adds;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            log_adds.push_back(alpha[t][i] + trans[i][j] + beta[t+1][j] + get_emission(x, y, j));    
        }
    }
    double sum = log_sum_exp(log_adds);

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            xi[i][j] += exp(log_adds[i*3+j] - sum);
        }
    }
}

// function for calculating the relative difference between values
double eval(double& prev_val, double next_val){
    double relative_diff = fabs(exp(prev_val) - exp(next_val));

    prev_val = next_val;

    return relative_diff;
}   

//function for estimating initial values for the model before the training
void estimate_initial_prob(vector<pair<string, string>> dataset){
    char prev_x, prev_y;
    int size;

    for(pair<string, string> pair : dataset){
        // starting pis and first part of emis
        if(pair.first[0] != '-' && pair.second[0] != '-'){
            pis[0]++;
            emission_M[convert_char_into_int(pair.first[0])][convert_char_into_int(pair.second[0])]++;
        } else if (pair.first[0] != '-'){
            pis[1]++;
            emission_Ix[convert_char_into_int(pair.first[0])]++;
        } else {
            pis[2]++;
            emission_Iy[convert_char_into_int(pair.second[0])]++;
        }

        prev_x = pair.first[0];
        prev_y = pair.second[0];

        size = max_size(pair.first.length());

        // starting trans
        for(int t = 1; t < size; t++){
            if(prev_x != '-' && prev_y != '-'){
                if(pair.first[t] != '-' && pair.second[t] != '-'){
                    trans[0][0]++;
                } else if (pair.first[t] != '-'){
                    trans[0][1]++;
                    //cout << "Here" << endl;
                } else {
                    trans[0][2]++;
                }
            } else if(prev_x != '-'){
                if(pair.first[t] != '-' && pair.second[t] != '-'){
                    trans[1][0]++;
                } else {
                    trans[1][1]++;
                }
            } else {
                if(pair.first[t] != '-' && pair.second[t] != '-'){
                    trans[2][0]++;
                } else {
                    trans[2][2]++;
                }
            }

            prev_x = pair.first[t];
            prev_y = pair.second[t];
            
            //prev are now curr
            if(prev_x != '-' && prev_y != '-'){
                emission_M[convert_char_into_int(prev_x)][convert_char_into_int(prev_y)]++;
            } else if(prev_x != '-'){
                emission_Ix[convert_char_into_int(prev_x)]++;
            } else {
                emission_Iy[convert_char_into_int(prev_y)]++;
            }
        }
    }

    int sum = 0;

    // starting pis prob calc
    for(int i = 0; i < 3; i++) sum += pis[i];
    for(int i = 0; i < 3; i++) pis[i] /= sum;

    // starting trans prob calc
    for(int i = 0; i < 3; i++){
        sum = 0;

        for(int j = 0; j < 3; j++) sum += trans[i][j];
        for(int j = 0; j < 3; j++) trans[i][j] /= sum;
    }
    sum = 0;

    // startign gamma_M
    for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++) sum += emission_M[i][j];
    for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++) emission_M[i][j] /= sum;
    
    //starting gamma_Ix and gamma_Iy
    int sum_x = 0, sum_y = 0;
    for(int i = 0; i < 4; i++){
        sum_x += emission_Ix[i];
        sum_y += emission_Iy[i];
    }
    for(int i = 0; i < 4; i++){
        emission_Ix[i] /= sum_x;
        emission_Iy[i] /= sum_y;
    }
}

/**
 * @brief Function for training the model using the Baum-Welch algorithm.
 * 
 * This function takes a pair of sequences and trains the model using the Baum-Welch algorithm 
 * for a set number of iterations or until the loss falls below a specified tolerance value.
 * 
 * @param dataset The pair of strings using the std::pair class from the C++ Standard Library.
 * @param max_iterations The maximum number of iterations for training the model.
 * @param tol The minimum tolerance value; training will stop when the loss is below this value.
 * 
 * @note The tol parameter should be less than 0.1.
 * @note The model may not be numerically stable.
 * @warning The dataset pairs must be of equal length.
 * 
 * @author Leon Hegedić
 */
void baum_welch(int max_iterations, double tol, pair<string, string> dataset){
    //init iter and size
    int iter = 0;
    int size; 
    double rel_diff = 1e9, temp = 0; 
    string x = dataset.first, y = dataset.second;

    // train loop
    while(iter <= max_iterations && rel_diff > tol){
        rel_diff = 0.0;

        //set size
        size = max_size(x.length());

        //set alpha and beta to init values
        set_alpha_beta(size);

        //reset xi for every pair
        reset_xi_and_gamma();
            
        // run forward and backward
        forward_algorithm(dataset);
        backward_algorithm(dataset);

        //calc gamma and xi
        for(int t = 0; t < size; t++){
            calc_gamma(t, size);
            if(t < size - 1)
                calc_xi(t, x[t+1], y[t+1]);
        }
        
        // calc gamma for each emission
        for(int t = 0; t < size; t++){
            if(convert_char_into_int(y[t]) == 4){
                gamma_Ix[convert_char_into_int(x[t])] += exp(gamma[t][1]);
            } else if(convert_char_into_int(x[t]) == 4){
                gamma_Iy[convert_char_into_int(y[t])] += exp(gamma[t][2]);
            } else {
                gamma_M[convert_char_into_int(x[t])][convert_char_into_int(y[t])] += exp(gamma[t][0]);
            }
        }

        for(int i = 0; i < size; i++){
            sum_gamma[0] += exp(gamma[i][0]);
            sum_gamma[1] += exp(gamma[i][1]);
            sum_gamma[2] += exp(gamma[i][2]);
        }

        //calc trans matrix values
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                temp = log(xi[i][j]) - log(sum_gamma[i]);
                rel_diff += eval(trans[i][j], temp);
            }
        }

        for(int state = 0; state < 3; state++){
            //calc pis
            rel_diff += eval(pis[state], gamma[0][state]);
            
            //calc emissions
            if(state == 0){
                for(int i = 0; i < 4; i++){
                    for(int j = 0; j < 4; j++){
                        rel_diff += eval(emission_M[i][j], log(gamma_M[i][j]) - log(sum_gamma[state]));
                    }
                }
            } else if(state == 1){
                for(int i = 0; i < 4; i++){
                    rel_diff += eval(emission_Ix[i], log(gamma_Ix[i]) - log(sum_gamma[state]));
                }
            } else if(state == 2){
                for(int i = 0; i < 4; i++){
                    rel_diff += eval(emission_Iy[i], log(gamma_Iy[i]) - log(sum_gamma[state]));
                }
            }
        }
    
        rel_diff = rel_diff / (36);
        cout << "t=" << iter << ": " << rel_diff << endl << endl;
        iter++;
    }
}