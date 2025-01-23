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
double xi_help[3][3];

// smoother
//double smooth_const = 0.0001;

//function for calculation gamma for each step t
void calc_gamma(int t, int size){
    double sum = 0.0;
    // calc gamma in step t
    for(int i = 0; i < 3; i++){
        sum+=(gamma[t][i] = alpha[t][i] * beta[t][i]);
        //if(gamma[t][i] <= smooth_const) gamma[t][i] = smooth_const; 
        //gamma[t][i] /= scaler[t];

       
    }

    for(int i = 0; i < 3; i++){
        gamma[t][i] /= sum;

        if(t < size - 1){
            sum_gamma[i] += gamma[t][i];
        }
    }
}

//reset xi and sum_xi for every iteration and every value
void reset_xi_and_gamma(){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            xi[i][j] = 0.0;
            xi_help[i][j] = 0.0;
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

//function for calculationg xi in each step t
void calc_xi(int t, char x, char y){
    double sum = 0.0;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(check_emission(x, y, j))
                sum += alpha[t][i] * trans[i][j] * beta[t+1][j] * get_emission(x, y, j);
                
        }

        //for(int j = 0; j < 3; j++){
        //    if(i == 2 && j == 1) continue;
        //    if(i == 1 && j == 2) continue;
        //    if(xi[i][j] <= smooth_const) xi[i][j] = smooth_const;
        //}
    }

    sum = (1.0 / sum);

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(check_emission(x, y, j))
                xi[i][j] += alpha[t][i] * trans[i][j] * beta[t+1][j] * get_emission(x, y, j) * sum;
                
        }

        //for(int j = 0; j < 3; j++){
        //    if(i == 2 && j == 1) continue;
        //    if(i == 1 && j == 2) continue;
        //    if(xi[i][j] <= smooth_const) xi[i][j] = smooth_const;
        //}
    }

    //for(int i = 0; i < 3; i++){
    //    for(int j = 0; j < 3; j++){
    //        if(check_emission(x, y, j))
    //            xi[i][j] += (xi_help[i][j] * (1.0 / sum));
    //            xi_help[i][j] = 0.0;
    //    }
    //}
}

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

// function for calculating the relative difference between values
double eval(double& prev_val, double next_val){
    double relative_diff = fabs(prev_val - next_val);

    prev_val = next_val;

    return relative_diff;
}   

void estimate_initial_prob(pair<string, string> dataset){
    char prev_x, prev_y;
    int size;

    // starting pis and first part of emis
    if(dataset.first[0] != '-' && dataset.second[0] != '-'){
        pis[0]++;
        emission_M[convert_char_into_int(dataset.first[0])][convert_char_into_int(dataset.second[0])]++;
    } else if (dataset.first[0] != '-'){
        pis[1]++;
        emission_Ix[convert_char_into_int(dataset.first[0])]++;
    } else {
        pis[2]++;
        emission_Iy[convert_char_into_int(dataset.second[0])]++;
    }

    prev_x = dataset.first[0];
    prev_y = dataset.second[0];

    size = max_size(dataset.first.length());

    // starting trans
    for(int t = 1; t < size; t++){
        if(prev_x != '-' && prev_y != '-'){
            if(dataset.first[t] != '-' && dataset.second[t] != '-'){
                trans[0][0]++;
            } else if (dataset.first[t] != '-'){
                trans[0][1]++;
                //cout << "Here" << endl;
            } else {
                trans[0][2]++;
            }
        } else if(prev_x != '-'){
            if(dataset.first[t] != '-' && dataset.second[t] != '-'){
                trans[1][0]++;
            } else {
                trans[1][1]++;
            }
        } else {
            if(dataset.first[t] != '-' && dataset.second[t] != '-'){
                trans[2][0]++;
            } else {
                trans[2][2]++;
            }
        }

        prev_x = dataset.first[t];
        prev_y = dataset.second[t];
        
        //prev are now curr
        if(prev_x != '-' && prev_y != '-'){
            emission_M[convert_char_into_int(prev_x)][convert_char_into_int(prev_y)]++;
        } else if(prev_x != '-'){
            emission_Ix[convert_char_into_int(prev_x)]++;
        } else {
            emission_Iy[convert_char_into_int(prev_y)]++;
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

//Baum-Welch algorithm for training the model
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
        size = max_size(dataset.first.size());

        //set alpha and beta to init values
        set_alpha_beta(size);

        //reset xi for every pair
        reset_xi_and_gamma();
            
        // run forward and backward
        forward_algorithm(dataset);
        backward_algorithm(dataset);

        //print_alpha_beta();

        //calc gamma and xi
        for(int t = 0; t < size; t++){
            calc_gamma(t, size);
            if(t < size - 1)
                calc_xi(t, dataset.first[t+1], dataset.second[t+1]);
        }
        
        //print_gamma();
        //print_xi();

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
                    temp = xi[i][j] / sum_gamma[i];
                rel_diff += eval(trans[i][j], temp);
                //cout << trans[i][j] << " ";

                //cout << rel_diff << " ";
            }
            //cout << endl;
            sum_gamma[i] += gamma[size - 1][i];
        }
        //cout << endl;

        for(int state = 0; state < 3; state++){
            //calc pis
            rel_diff += eval(pis[state], gamma[0][state]);
            //cout << rel_diff << endl;
            
            //calc emissions
            if(state == 0){
                for(int i = 0; i < 4; i++){
                    for(int j = 0; j < 4; j++){
                        rel_diff += eval(emission_M[i][j], gamma_M[i][j] / sum_gamma[state]);
                        //cout << rel_diff << " ";
                    }
                    //cout << endl;
                }
                //cout << endl;
            } else if(state == 1){
                for(int i = 0; i < 4; i++){
                    rel_diff += eval(emission_Ix[i], gamma_Ix[i] / sum_gamma[state]);
                    //cout << rel_diff << " ";
                }
                //cout << endl << endl;
            } else if(state == 2){
                for(int i = 0; i < 4; i++){
                    rel_diff += eval(emission_Iy[i], gamma_Iy[i] / sum_gamma[state]);
                //cout << rel_diff << " "; 
                }
            //cout << endl << endl;
            }
        }

        rel_diff /= 36;
        cout << "t=" << iter << ": " << rel_diff << endl << endl;
        //cout << "Pair diff=" << rel_diff << endl << endl;    
        iter++;
        print_model_params();
        cout << endl;
    }
}