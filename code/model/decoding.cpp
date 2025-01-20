#include "model.h"
#include "utils.h"

using namespace std;

void viterbi(pair<string, string> pair, const string& file_path){

    string x = pair.first;
    string y = pair.second;
    int n = x.length();
    int m = y.length();

    vector<vector<double>> log_M(n, vector<double>(m, 0.0));
    vector<vector<double>> log_Ix(n, vector<double>(m, 0.0));
    vector<vector<double>> log_Iy(n, vector<double>(m, 0.0));

    log_Ix[0][0] = numeric_limits<double>::lowest();
    log_Iy[0][0] = numeric_limits<double>::lowest();

    for(int i = 1; i < n + 1; i++){
        log_Ix[i][0] = i * (log(trans[1][1])) + log(get_emission(x[i-1], '-', 1));
        log_Iy[i][0] = numeric_limits<double>::lowest();
        log_M[i][0] = numeric_limits<double>::lowest();
    }

    for(int j = 1; j < m + 1; j++){
        log_Iy[0][j] = j * (log(trans[2][2])) + log(get_emission('-', y[j-1], 2));
        log_Ix[0][j] = numeric_limits<double>::lowest();
        log_M[0][j] = numeric_limits<double>::lowest();
    }

    for(int i = 1; i < n + 1; i++){
        for(int j = 1; j < m + 1; j++){
            log_M[i][j] = log(get_emission(x[i-1], y[j-1], 0)) + max({log_M[i-1][j-1], log_Ix[i-1][j-1], log_Iy[i-1][j-1]});
            log_Ix[i][j] = log(get_emission(x[i-1], '-', 1)) + max({log(trans[0][1]) + log_M[i-1][j], log(trans[2][1]) + log_M[i-1][j]});
            log_Iy[i][j] = log(get_emission('-', y[j-1], 2)) + max({log(trans[0][2]) + log_M[i][j-1], log(trans[1][2]) + log_M[i][j-1]});
        }
    }

    string align_x = "", align_y = "";

    while(n > 0 || m > 0){
        if(log_M[n][m] >= log_Ix[n][m] && log_M[n][m] >= log_Iy[n][m]){
            align_x += x[n-1];
            align_y += y[m-1];
            n--;
            m--;
        } else if(log_Ix[n][m] > log_M[n][m] && log_Ix[n][m] >= log_Iy[n][m]){
            align_x += x[n-1];
            align_y += '-';
            n--;
        } else {
            align_x += '-';
            align_y += y[m-1];
            m--;
        }
    }

    reverse(align_x.begin(), align_x.end());
    reverse(align_y.begin(), align_y.end());

    save_pair_into_file(file_path, make_pair(align_x, align_y));
}