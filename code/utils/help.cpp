#include "../headers/model.h"
#include "../headers/utils.h"

using namespace std;

// funtion for printing first 10 forward and backward probabilities
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

//funtion for printg model params
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