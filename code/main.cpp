#include<getopt.h>
#include <sstream>

#include "headers/model.h"
#include "headers/needleman_wunsch.h"
#include "headers/utils.h"

using namespace std;

int main(int argc, char* argv[]){
    int opt;
    string file_save = "", mode = "", file_name = "";
    int max_iter = 20;
    double tol = 0.01;

    while((opt = getopt(argc, argv, "s:m:f:b:t:")) != -1)  
    {  
        if(optarg == NULL){
            cout << "error" << endl;
        }
        switch(opt)  
        {
            case 's':
            file_save = optarg;
            break;

            case 'm':
            mode = optarg;
            break;

            case 'f':
            file_name = optarg;
            break;

            case 'b':
            stringstream(optarg) >> max_iter;
            break;

            case 't':
            stringstream(optarg) >> tol;
            break;

            default:
            cout << "Invalid argument given!"<< endl;
            return 1;
        }  
    }

    if(file_save == "" || file_name == "" || mode == ""){
        cout << "Arguments s, f and m are mandatory!" << endl;
        return 1;
    }

    if(mode == "train"){
        pair<string, string> dataset = read_file_into_pair(file_name);
        //estimate_initial_prob(dataset);
        get_model_params_from_file("model.txt");
        baum_welch(max_iter, tol, dataset);
        //save_model_params_to_file("model.txt");
    } else if(mode == "viterbi"){
        get_model_params_from_file("model.txt");
        pair<string, string> pair = read_file_into_pair(file_name);
        viterbi(pair, file_save);
    } else if(mode == "needleman"){
        pair<string, string> pair = read_file_into_pair(file_name);
        needleman_wunsch(pair, file_save);
    } else {
        cout << "Invalid argument for m given!" << endl;
        return 1;
    }

    return 0;
}