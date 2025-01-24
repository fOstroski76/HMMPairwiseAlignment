#include<getopt.h>
#include <sstream>

#include "headers/model.h"
#include "headers/needleman_wunsch.h"
#include "headers/utils.h"

using namespace std;

int main(int argc, char* argv[]){
    get_model_params_from_file("model.txt");

    pair<string, string> pair = read_file_into_pair("train/training_no_0.txt");

    forward_algorithm(pair);
    backward_algorithm(pair);

    print_alpha_beta();
    return 0;
}