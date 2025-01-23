#include "../headers/model.h"
#include "../headers/utils.h"

using namespace std;

void print_model_params(){
    cout << "Pis:" << endl;
    for(int i = 0; i < 3; i++){
        cout << pis[i] << " ";
    }

    cout << endl << "Trans:" << endl;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            cout << trans[i][j] << " ";
        }
        cout << endl;
    }

    cout << "emission_M" << endl;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cout << emission_M[i][j] << " ";
        }
        cout << endl;
    }

    cout << "emission_Ix" << endl;
    for(int j = 0; j < 4; j++){
        cout << emission_Ix[j] << " ";
    }
    cout << endl;

    cout << "emission_Iy" << endl;
    for(int j = 0; j < 4; j++){
        cout << emission_Ix[j] << " ";
    }
    cout << endl;
}

void viterbi(pair<string, string> pair, const string& file_path){
    string x = pair.first;
    string y = pair.second;
    int n = x.length();
    int m = y.length();

    double epsilon = (log(trans[1][1]) + log(trans[2][2]))/2;
    double delta = (log(trans[0][1]) + log(trans[0][2]))/2;

    vector<vector<int>> path_M(n+1, vector<int>(m+1, 0));
    vector<vector<int>> path_Ix(n+1, vector<int>(m+1, 0));
    vector<vector<int>> path_Iy(n+1, vector<int>(m+1, 0));

    vector<double> v_M_prev(m+1, numeric_limits<double>::lowest());
    vector<double> v_Ix_prev(m+1, numeric_limits<double>::lowest());
    vector<double> v_Iy_prev(m+1, numeric_limits<double>::lowest());
    v_M_prev[0] = 0.0;

    vector<double> v_M_curr(m+1, 0.0);
    vector<double> v_Ix_curr(m+1, 0.0);
    vector<double> v_Iy_curr(m+1, 0.0);

    v_M_curr[0] = numeric_limits<double>::lowest();
    v_Ix_curr[0] = numeric_limits<double>::lowest();
    v_Iy_curr[0] = numeric_limits<double>::lowest();

    for(int i = 1; i < n + 1; i++){
        for(int j = 1; j < m + 1; j++){

            if(v_M_prev[j-1] >= v_Ix_prev[j-1] && v_M_prev[j-1] >= v_Iy_prev[j-1]){ 
                v_M_curr[j] = log(get_emission(x[i-1], y[i-1], 0)) + v_M_prev[j-1];
                path_M[i][j] = 0;
                //cout << "Curr=" << log(get_emission(x[i-1], y[j-1], 0)) << " " << v_M_prev[j-1] << endl;
            } else if(v_Ix_prev[j-1] >= v_M_prev[j-1] && v_Ix_prev[j-1] >= v_Iy_prev[j-1]){
                v_M_curr[j] = log(get_emission(x[i-1], y[i-1], 0)) + v_Ix_prev[j-1];
                path_M[i][j] = 1;
            } else if(v_Iy_prev[j-1] >= v_Ix_prev[j-1] && v_Iy_prev[j-1] >= v_M_prev[j-1]){
                v_M_curr[j] = log(get_emission(x[i-1], y[i-1], 0)) + v_Iy_prev[j-1];
                path_M[i][j] = 2;
            }

            if(v_M_prev[j] + delta >= v_Ix_prev[j] + epsilon){
                v_Ix_curr[j] = log(get_emission(x[i-1], '-', 1)) + v_M_prev[j] + delta;
                path_Ix[i][j] = 0;
            } else if(v_M_prev[j] + delta < v_Ix_prev[j] + epsilon){
                v_Ix_curr[j] = log(get_emission(x[i-1], '-', 1)) + v_Ix_prev[j] + epsilon;
                path_Ix[i][j] = 1;
            }

            if(v_M_curr[j-1] + delta >= v_Iy_curr[j-1] + epsilon){
                v_Iy_curr[j] = log(get_emission('-', y[i-1], 2)) + v_M_curr[j-1] + delta;
                path_Ix[i][j] = 0;
            } else if(v_M_curr[j-1] + delta < v_Iy_curr[j-1] + epsilon){
                v_Iy_curr[j] = log(get_emission('-', y[i-1], 2)) + v_Iy_curr[j-1] + epsilon;
                path_Ix[i][j] = 2;
            }
        }
        // for(int j = 0; j < m+1; j++){
        //     cout << v_M_curr[i] << " ";
        // }
        // cout <<"|"<< endl;

        // for(int j = 0; j < m+1; j++){
        //     cout << v_Ix_curr[i] << " ";
        // }
        // cout <<"|"<< endl;

        // for(int j = 0; j < m+1; j++){
        //     cout << v_Iy_curr[i] << " ";
        // }
        // cout <<"|"<< endl << endl;

        for(int j = 0; j < m+1; j++){
            v_M_prev[j] = v_M_curr[j];
            //cout << v_M_curr[j] << " ";
            v_Ix_prev[j] = v_Ix_curr[j];
            //cout << v_Ix_curr[j] << " ";
            v_Iy_prev[j] = v_Iy_curr[j];
            //cout << v_Iy_curr[j] << endl;
        }
        //cout << endl;
    }

    //cout << "Here" << endl;

    string align_x = "", align_y = "", result = "";
    vector<vector<int>> path;
    if(v_M_prev[m] >= v_Ix_prev[m] && v_M_prev[m] >= v_Iy_prev[m]){
        path = path_M;
    } else if(v_Ix_prev[m] >= v_M_prev[m] && v_Ix_prev[m] >= v_Iy_prev[m]){
        path = path_Ix;
    } else {
        path = path_Iy;
    }

    int state = path[n][m];
    //int i = n;
    //int j = m;
    while(n > 0 || m > 0){
        cout << n << " " << m << endl;
        if(state == 0){
            align_x = x[n-1] + align_x;
            align_y = y[m-1] + align_y;
            n--;
            m--;
            path = path_M;
            state = path[n][m];
        } else if(state == 1){
            align_x = x[n-1] + align_x;
            align_y = '-' + align_y;
            n--;
            path = path_Ix;
            state = path[n][m];
        } else {
            align_x = '-' + align_x;
            align_y = y[m-1] + align_y;
            m--;
            path = path_Iy;
            state = path[n][m];
        }
    }

    // reverse(result.begin(), result.end());
    // for(char c : result){
    //     switch(c){
    //         case 'M':
    //         align_x += x[i];
    //         align_y += y[j];
    //         i++;
    //         j++;
    //         break;

    //         case 'X':
    //         align_x += x[i];
    //         align_y += '-';
    //         i++;
    //         break;

    //         default:
    //         align_x += '-';
    //         align_y += y[j];
    //         j++;
    //         break;
    //     }
    // }

    save_pair_into_file(file_path, make_pair(align_x, align_y));
}