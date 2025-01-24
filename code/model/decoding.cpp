#include "../headers/model.h"
#include "../headers/utils.h"

using namespace std;

void print_model_params(){
    cout << "Pis:" << endl;
    for(int i = 0; i < 3; i++){
        cout << exp(pis[i]) << " ";
    }

    cout << endl << "Trans:" << endl;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            cout << exp(trans[i][j]) << " ";
        }
        cout << endl;
    }

    cout << "emission_M" << endl;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cout << exp(emission_M[i][j]) << " ";
        }
        cout << endl;
    }

    cout << "emission_Ix" << endl;
    for(int j = 0; j < 4; j++){
        cout << exp(emission_Ix[j]) << " ";
    }
    cout << endl;

    cout << "emission_Iy" << endl;
    for(int j = 0; j < 4; j++){
        cout << exp(emission_Ix[j]) << " ";
    }
    cout << endl;
}

void viterbi(pair<string, string> pair, const string& file_path){
    string x = pair.first;
    string y = pair.second;
    int n = x.length();
    int m = y.length();

    double epsilon = (trans[1][1] + trans[2][2])/2;
    double delta = (trans[0][1] + trans[0][2])/2;

    vector<vector<double>> log_M(n+1, vector<double>(m+1, 0.0));
    vector<vector<double>> log_X(n+1, vector<double>(m+1, 0.0));
    vector<vector<double>> log_Y(n+1, vector<double>(m+1, 0.0));

    log_M[0][0] = 0.0;
    log_X[0][0] = -numeric_limits<double>::infinity();
    log_Y[0][0] = -numeric_limits<double>::infinity();

    for(int i = 1; i < n + 1; i++){
        log_M[i][0] = -numeric_limits<double>::infinity();
        log_X[i][0] = -numeric_limits<double>::infinity();//i * epsilon + delta;
        log_Y[i][0] = -numeric_limits<double>::infinity();
    }

    for(int j = 1; j < m + 1; j++){
        log_M[0][j] = -numeric_limits<double>::infinity();
        log_X[0][j] = -numeric_limits<double>::infinity();
        log_Y[0][j] = -numeric_limits<double>::infinity();//j * epsilon + delta;
    }

    double v1, v2, v3, v4, v5, v6, v7;

    for(int i = 1; i < n + 1; i++){
        for(int j = 1; j < m + 1; j++){
            v1 = log_M[i-1][j-1];
            v2 = log_X[i-1][j-1];
            v3 = log_Y[i-1][j-1];

            if(v1 >= v2 && v1 >= v3){
                log_M[i][j] = v1 + get_emission(x[i-1], y[j-1], 0); 
            } else if(v2 > v1 && v2 >= v3){
                log_M[i][j] = v2 + get_emission(x[i-1], y[j-1], 0);
            } else {
                log_M[i][j] = v3 + get_emission(x[i-1], y[j-1], 0);
            }

            v4 = log_M[i-1][j] + delta;
            v5 = log_X[i-1][j] + epsilon;

            if(v4 >= v5){
                log_X[i][j] = v4 + get_emission(x[i-1], '-', 1);
            } else {
                log_X[i][j] = v5 + get_emission(x[i-1], '-', 1);
            }

            v6 = log_M[i][j-1] + delta;
            v7 = log_Y[i][j-1] + epsilon;
            
            if(v6 >= v7){
                log_Y[i][j] = v6 + get_emission('-', y[j-1], 2);
            } else {
                log_Y[i][j] = v7 + get_emission('-', y[j-1], 2);
            }
        }
    }

    double score = max({log_M[n][m], log_X[n][m], log_Y[n][m]});
    string align_x = "", align_y = "";
    double max_val = 0;
    while(n > 0 || m > 0){
        max_val = max({log_M[n][m], log_X[n][m], log_Y[n][m]});
        if(max_val == log_M[n][m]){
            align_x = x[n-1] + align_x;
            align_y = y[m-1] + align_y;

            n--;
            m--;
        } else if(max_val == log_X[n][m]){
            align_x = x[n-1] + align_x;
            align_y = '-' + align_y;

            n--;
        } else {
            align_x = '-' + align_x;
            align_y = y[m-1] + align_y;

            m--;
        }
    }

    cout << "Viterbi score is: " << score << endl;
    save_pair_into_file(file_path, make_pair(align_x, align_y));
}