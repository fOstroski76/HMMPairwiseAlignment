#include<getopt.h>
#include <sstream>

#include "headers/model.h"
#include "headers/needleman_wunsch.h"
#include "headers/utils.h"

using namespace std;

int main(int argc, char* argv[]){
    int opt;
    string file_save = "", dir_name = "", mode = "", file_name = "";
    int max_iter = 20;
    double tol = 0.0001;

    while((opt = getopt(argc, argv, "s:m:f:b:t:d:")) != -1)  
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

            case 'd':
            stringstream(optarg) >> dir_name;
            break;

            default:
            cout << "Invalid argument given!"<< endl;
            return 1;
        }  
    }

    if(mode == ""){
        cout << "Argument for m is mandatory!" << endl;
        return 1;
    }

    if(mode == "train"){

        if(dir_name == ""){
            cout << "Direcotry name was not given!" << endl;
            return 1;
        }

        vector<pair<string, string>> dataset = get_training_data_from_folder(file_name);
        estimate_initial_prob(dataset);

        string x = "";
        string y = "";
        for(pair<string, string> pair : dataset){
            x += pair.first;
            y += pair.second;
        }
        dataset.clear();

        baum_welch(max_iter, tol, make_pair(x, y));

        if(file_save == "" || file_save == "model"){
            save_model_params_to_file("model");
        } else {
            save_model_params_to_file(file_save);
        }

    } else if(mode == "viterbi"){

        if(file_name == ""){
            cout << "File name was not given!" << endl;
            return 1;
        }

        get_model_params_from_file("model");
        pair<string, string> pair = read_file_into_pair(file_name);
        if(file_save == ""){
            viterbi(pair, "viterbi_result.txt");        
        } else { 
            viterbi(pair, file_save);  
        }

    } else if(mode == "needleman"){

        if(file_name == ""){
            cout << "File name was not given!" << endl;
            return 1;
        }

        pair<string, string> pair = read_file_into_pair(file_name);

        if(file_save == ""){
            needleman_wunsch(pair, "needleman_wunsch_result.txt");        
        } else { 
            needleman_wunsch(pair, file_save);  
        }

    } else {

        cout << "Invalid argument for m given!" << endl;
        return 1;

    }

    return 0;
}