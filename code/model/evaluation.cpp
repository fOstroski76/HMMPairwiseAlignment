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
double scaler[MAX_SIZE];

void set_alpha_beta(int size){
    for(int t = 0; t < size; t++){
        for(int i = 0; i < 3; i++){
            alpha[t][t] = 0.0;
            beta[t][i] = 0.0;
        }

        scaler[t] = 0.0;
    }
}

void print_alpha_beta(){
    //print alpha
    cout << "Alpha: " << endl;
    for(int t = 0; t < 10; t++){
        for (int i = 0; i < 3; i++){
            cout << alpha[t][i] << " ";
        }
        cout << endl;
    }

    cout << "Beta: " << endl;
    for(int t = 0; t < 10; t++){
        for (int i = 0; i < 3; i++){
            cout << beta[t][i] << " ";
        }
        cout << endl;
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
    if(!check_emission(xt, yt, state))  return 0.0;

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
    ofstream file("data.txt", ios::app);
    file << "ALPHA" << endl;

    string x = pairs.first;
    string y = pairs.second;

    //init size and sum
    int size = max_size(x.length());
    double sum = 0.0;

    //init alpha values
    //file << "t=" << 0 << " | ";
    for(int i = 0; i < 3; i++){
        if(check_emission(x[0], y[0], i)){
            alpha[0][i] = pis[i] * get_emission(x[0], y[0], i);
            scaler[0] += alpha[0][i];
        }
    }

    scaler[0] = (1.0 /scaler[0]);

    //scaling scheme
    for(int i = 0; i < 3; i++){
        alpha[0][i] *= scaler[0];
        //file << alpha[0][i] <<  " ";
    }
    //file << scaler[0] << endl;

    //iterate over alpha for each t
    for(int t = 1; t < size; t++){
        file << "t=" << t << " || ";
        // calc alpha in t
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(check_emission(x[t], y[t], i)){
                    alpha[t][i] += alpha[t-1][j] * trans[j][i] * get_emission(x[t], y[t], i);
                    file << alpha[t-1][j] << " " << trans[j][i]  << " " << get_emission(x[t], y[t], i)  << " | ";
                }
            }
            scaler[t] += alpha[t][i];
        }

        scaler[t] = (1.0 / scaler[t]);
        
        //scale
        for(int i = 0; i < 3; i++){
            alpha[t][i] *= scaler[t];
            file << alpha[t][i] << " ";
        }
        file << " || " << scaler[t] << endl;
    }
    file.close();
}

void backward_algorithm(pair<string, string> pairs){
    ofstream file("data.txt", ios::app);
    file << "BETA" << endl;

    string x = pairs.first;
    string y = pairs.second;
    double sum = 0;

    //init size and sum
    int size = max_size(x.length());

    //file << "t=" << size-1 << " | ";
    //init beta in T-1 time
    for(int i = 0; i < 3; i++){
        beta[size - 1][i] = 1.0 / 3;//scaler[size-1];
        file << beta[size - 1][i] << " ";
    }
    file << endl;
    //file << " || " << scaler[0] << endl;

    //iterate from T-1 to 0 for each beta
    for(int t = size - 2; t >= 0; t--){
        sum = 0.0;
        file << "t=" << t << " | ";

        //calc beta * emis in t
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(check_emission(x[t+1], y[t+1], j)){
                    beta[t][i] += beta[t+1][j] * trans[i][j] * get_emission(x[t+1], y[t+1], j);
                    //file << beta[t][i] << " ";//beta[t+1][j] << " " << trans[i][j]  << " " << get_emission(x[t+1], y[t+1], j)  << " | ";
                }
            }
            if(check_emission(x[t], y[t], i))
                sum += beta[t][i] * get_emission(x[t], y[t], i);
            //beta[t][i] *= scaler[t];
            //sum += beta[t][i];
            //file << scaler[t] << " = " <<  beta[t][i] << " || ";
        }
        //file << sum << " | ";//scaler[t] << endl;

        for(int i = 0; i < 3; i++){
            if(check_emission(x[t], y[t], i))
                beta[t][i] *= (get_emission(x[t], y[t], i) / sum);
            file << beta[t][i] << " ";
        }

        file << endl;
    }

    file.close();
}