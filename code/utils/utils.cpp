#include "utils.h"
#include "model.h"

using namespace std;

int max_size(int seq_size){
    const int seq_len = seq_size;
    return min(MAX_SIZE, seq_len);
}

int convert_char_into_int(char c){
    switch (c)
    {
    case 'A':
        return 0;
    case 'C':
        return 1;
    case 'G':
        return 2;
    case 'T':
        return 3;
    case '-':
        return 4;
    default:
        break;
    }
}

vector<pair<string, string>> get_file_name_into_folder(const string& folder_path) {
    vector<string> file_names;

    try {
        // Iterate through the folder
        for (const auto& entry : filesystem::directory_iterator(folder_path)) {
            if (filesystem::is_regular_file(entry.path())) { 
                file_names.push_back(entry.path().filename().string());
            }
        }
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Error accessing folder: " << e.what() << endl;
    }

    vector<pair<string, string>> sequance_container;

    for(string name : file_names){
        sequance_container.push_back(read_file_into_pair(name));
    }

    return sequance_container;
}

pair<string, string> read_file_into_pair(const string& file_path) {
    ifstream file(file_path);
    
    if (!file.is_open()) {
        throw runtime_error("Error: Could not open file " + file_path);
    }

    string line1 = "", line2 = "";

    // Read the first line
    if (!getline(file, line1)) {
        throw runtime_error("Error: File does not contain the first line.");
    }

    // Read the second line
    if (!getline(file, line2)) {
        throw runtime_error("Error: File does not contain the second line.");
    }

    return make_pair(line1, line2);
}

void save_pair_into_file(const string& file_path, pair<string, string> aligned_seq){
    ofstream file(file_path);

    if(!file.is_open()){
        throw runtime_error("Error: Could not open file " + file_path);
    }

    file << aligned_seq.first << endl;
    file << aligned_seq.second << endl;

    file.close();
}

void get_model_params_from_file(const string& file_path){
    ifstream file(file_path);

    if (!file.is_open()) {
        throw runtime_error("Error: Could not open file " + file_path);
    }

    string param = "";
    try {
        // get pis
        for(int i = 0; i < 3; i++){
            if (!getline(file, param)) {
                throw runtime_error("Error: File does not contain the first line.");
            }

            pis[i] = stod(param);
        }

        // get trans
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if (!getline(file, param)) {
                    throw runtime_error("Error: File does not contain the first line.");
                }

                trans[i][j] = stod(param);
            }
        }

        //get M state emissions
        for(int i = 0;  i < 4; i++){
            for(int j = 0;  j < 4; j++){
                if (!getline(file, param)) {
                    throw runtime_error("Error: File does not contain the first line.");
                }

                emission_M[i][j] = stod(param);
            }
        }

        //get Ix state emissions
        for(int i = 0; i < 4; i++){
            if (!getline(file, param)) {
                throw runtime_error("Error: File does not contain the first line.");
            }

            emission_Ix[i] = stod(param);
        }

        //get Ix state emissions
        for(int i = 0; i < 4; i++){
            if (!getline(file, param)) {
                throw runtime_error("Error: File does not contain the first line.");
            }

            emission_Iy[i] = stod(param);
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid argument. The string cannot be converted to a double." << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Out of range. The string represents a number too large to store in a double." << std::endl;
    }
    file.close();
}

void save_model_params_to_file(const string& file_path){
    ofstream out_file(file_path);

    if (!out_file.is_open()) {
        throw runtime_error("Error: Could not open file " + file_path);
    }

    //save pis
    for(int i = 0; i < 3; i++){
        out_file << pis[i] << endl;
    }

    // save trans
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            out_file << trans[i][j] << endl;
        }
    }

    //save M state emissions
    for(int i = 0;  i < 4; i++){
        for(int j = 0;  j < 4; j++){
            out_file << emission_M[i][j] << endl;;
        }
    }

    //save Ix state emissions
    for(int i = 0; i < 4; i++){
        out_file << emission_Ix[i] << endl;
    }

    //save Ix state emissions
    for(int i = 0; i < 4; i++){
        out_file << emission_Iy[i] << endl;
    }

    out_file.close();
}