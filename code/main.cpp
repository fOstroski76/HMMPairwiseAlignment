#include<getopt.h>
#include <sstream>

#include "headers/model.h"
#include "headers/needleman_wunsch.h"
#include "headers/utils.h"

using namespace std;

int main(int argc, char* argv[]){
    int opt;
    string file_save = "", mode = "", file_name = "";
    int max_iter = 2000;
    double tol = 0.01;

    while((opt = getopt(argc, argv, ":nmfbt")) != -1)  
    {  
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
        vector<pair<string, string>> dataset = get_file_name_into_folder(file_name);
        estimate_initial_prob(dataset);
        baum_welch(max_iter, tol, dataset);
        save_model_params_to_file(file_save);
    } else if(mode == "viterbi"){
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