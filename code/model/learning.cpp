#include "model.h"
#include "utils.h"

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
double sum_xi[3][3];

//function for calculation gamma for each step t
void calc_gamma(int t, int size){
    double sum = 0;

    // calc gamma in step t
    for(int i = 0; i < 3; i++){
        sum += (gamma[t][i] = alpha[t][i] * beta[t][i]);
    }

    //normalize gamma in step t
    for(int i = 0; i < 3; i++){
        gamma[t][i] /= sum;
        if(t < size - 1)
            sum_gamma[i] += gamma[t][i];
    }
}

//reset xi and sum_xi for every iteration and every value
void reset_xi(){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            sum_xi[i][j] = 0;
            xi[i][j] = 0;
        }
    }
}

//function for calculationg xi in each step t
void calc_xi(int t, char x, char y){
    double sum = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            sum += (xi[i][j] = alpha[t][i] * trans[i][j] * beta[t+1][j] * get_emission(x, y, j));
        }
    }

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            sum_xi[i][j] += (xi[i][j] /= sum);
}

// function for calculating the relative difference between values
double eval(double& prev_val, double next_val){
    double relative_diff = fabs((prev_val - next_val) / prev_val);

    prev_val = next_val;

    return relative_diff;
}   

void estimate_initial_prob(vector<pair<string, string>> dataset){
    char prev_x, prev_y;
    int size;

    for(pair<string, string>pair : dataset){
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
                } else if (pair.first[0] != '-'){
                    trans[0][1]++;
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
    for(int i = 0; i < 3; i++) pis[i] /= dataset.size();

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

//Baum-Welch algorithm for training the model
void baum_welch(int max_iterations, double tol, vector<pair<string, string>> dataset){
    //init iter and size
    int iter = 0;
    int size; 
    double rel_diff = 1e9, temp = 0; 
    string x, y;

    // train loop
    while(iter <= max_iterations || rel_diff < tol){
        cout << "t=" << iter << ": " << rel_diff << endl;
        //calc diff
        rel_diff = 0;

        // goding over each pair
        for(pair<string, string> pair : dataset){
            //set size
            size = max_size(pair.first.size());

            //reset xi for every pair
            reset_xi();
            
            // run forward and backward
            forward_algorithm(pair);
            backward_algorithm(pair);

            //calc gamma and xi
            for(int t = 0; t < size; t++){
                calc_gamma(t, size);
                if(t < size - 1)
                    calc_xi(t, pair.first[t+1], pair.second[t+1]);
            }

            x = pair.first;
            y = pair.second;

            // calc gamma for each emission
            for(int t = 0; t < size; t++){
                if(convert_char_into_int(y[t]) == 4){
                    gamma_Ix[convert_char_into_int(x[t])] += gamma[t][1];
                } else if(convert_char_into_int(x[t]) == 4){
                    gamma_Iy[convert_char_into_int(y[t])] += gamma[t][2];
                } else {
                    gamma_M[convert_char_into_int(x[t])][convert_char_into_int(y[t])] += gamma[t][0];
                }
            }

            //calc trans matrix values
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    temp = sum_xi[i][j] / sum_gamma[i];
                    rel_diff += eval(trans[i][j], temp);
                }

                sum_gamma[i] += gamma[size - 1][i];
            }

            for(int i = 0; i < 3; i++){
                //calc pis
                rel_diff += eval(pis[i], gamma[0][i]);

                for(int t = 0; t < size; t++){
                    
                    //calc emissions
                    if(check_emission(x[t], y[t], i) && i == 0){
                        rel_diff += eval(get_emission(x[t], y[t], i), gamma_M[convert_char_into_int(x[t])][convert_char_into_int(y[t])] / sum_gamma[i]);
                    } else if(check_emission(x[t], y[t], i) && i == 1){
                        rel_diff += eval(get_emission(x[t], y[t], i), gamma_Ix[convert_char_into_int(x[t])] / sum_gamma[i]);
                    } else if(check_emission(x[t], y[t], i) && i == 2){
                        rel_diff += eval(get_emission(x[t], y[t], i), gamma_Iy[convert_char_into_int(y[t])] / sum_gamma[i]);
                    }
                }
            }
        }

        rel_diff /= dataset.size(); 
        iter ++;
    }
}