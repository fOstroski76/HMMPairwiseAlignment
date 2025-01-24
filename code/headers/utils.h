#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <filesystem>
#include <utility>
#include <fstream>
#include <ostream>

using namespace std;

const double log_const = 1.0e-10;

// util functions
double safe_log(double value);
double log_sum_exp(const vector<double>& logValues);
int max_size(int seq_size);
int convert_char_into_int(char c);

// read, write seq
vector<pair<string, string>> get_file_name_into_folder(const string& folder_path);
pair<string, string> read_file_into_pair(const string& file_path);
void save_pair_into_file(const string& file_path, pair<string, string> aligned_seq);

// read, write model 
void get_model_params_from_file(const string& file_path);
void save_model_params_to_file(const string& file_path);

#endif