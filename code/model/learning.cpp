#include "model.h"
#include "utils.h"

using namespace std;

//gammas
double gamma[MAX_SIZE][3];
double sum_gamma[3];
double sum_em_gamma[3][5][5];

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

//function for calculationg xi in each step t
void calc_xi(int t, int x, int y){
    double sum = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            sum += (xi[i][j] = alpha[t][i] * trans[i][j] * beta[t+1][j] * emissions[j][x][y]);
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

//Baum - welch for training the model
void baum_welch(int max_iterations, double tol, vector<int> x, vector<int> y){
    //init iter and size
    int iter = 0;
    int size = max_size(x.size());
    double rel_diff = 1e9, temp = 0; 

    // train loop
    while(iter <= max_iterations || rel_diff < tol){
        cout << "t=" << iter << ": " << rel_diff << endl;

        //calc diff
        rel_diff = 0;

        // run forward and backward
        forward_algorithm(x, y);
        backward_algorithm(x, y);

        //calc gamma and xi
        for(int t = 0; t < size; t++){
            calc_gamma(t, size);
            if(t < size - 1)
                calc_xi(t, x[t+1], y[t+1]);
        }

        // calc gamma for each emission
        for(int t = 0; t < size; t++){
            if(y[t] == 4){
                sum_em_gamma[1][x[t]][y[t]] += gamma[t][1];
            } else if(x[t] == 4){
                sum_em_gamma[2][x[t]][y[t]] += gamma[t][2];
            } else {
                sum_em_gamma[0][x[t]][y[t]] += gamma[t][0];
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
                rel_diff += eval(emissions[i][x[t]][y[t]], sum_em_gamma[i][x[t]][y[t]] / sum_gamma[i]);
            }
        }
        
        iter ++;
    }
}