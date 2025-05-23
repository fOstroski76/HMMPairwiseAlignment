#include "../headers/needleman_wunsch.h"
#include "../headers/utils.h"

using namespace std;

/**
 * @brief Function for aligning sequences using the Needleman-Wunsch algorithm.
 * 
 * This function takes a pair of sequences and aligns them using the Needleman-Wunsch algorithm. 
 * The aligned sequences are then saved to the specified file path.
 * 
 * @param pair The pair of strings to be aligned, using the std::pair class from the C++ Standard Library.
 * @param file_path The file path where the aligned sequences will be saved.
 * 
 * @author Leon Hegedić
 */
void needleman_wunsch(pair<string, string> pair, const string& file_path){ 
    string x = pair.first;
    string y = pair.second;
    int n = x.length();
    int m = y.length();

    vector<vector<int>> matrix (n + 1, vector<int>(m + 1, 0));
    int match = 1, mismatch = -1, gap = -1;

    string aligned_x = "", aligned_y = "";
    int diag = 0, top = 0, left = 0;

    for(int i = 1; i < n + 1; i++) matrix[i][0] = matrix[i - 1][0] + gap;
    for(int j = 1; j < m + 1; j++) matrix[0][j] = matrix[0][j - 1] + gap;

    for(int i = 1; i < n+1; i++){
        for(int j = 1; j < m+1; j++){
            if(x[i - 1] == y[j - 1]){
                diag = matrix[i-1][j-1] + match;
            } else {
                diag = matrix[i-1][j-1] + mismatch;
            }

            top = matrix[i][j-1] + gap;
            left = matrix[i-1][j] + gap;

            matrix[i][j] = max({top, left, diag});
        }
    }

    int ret = matrix[n][m];
    while(n > 0 || m > 0){
        if(n > 0 
        && m > 0 
        && (x[n-1] == y[m-1] ? matrix[n][m] == matrix[n-1][m-1] + match : matrix[n][m] == matrix[n-1][m-1] + mismatch)){
            aligned_x = x[n-1] + aligned_x;
            aligned_y = y[m-1] + aligned_y;
            n--;
            m--;
        } else if(n > 0 && matrix[n][m] == matrix[n-1][m] + gap){
            aligned_x = x[n-1] + aligned_x;
            aligned_y = '-' + aligned_y;
            n--;
        } else {
            aligned_x = '-' + aligned_x;
            aligned_y = y[m-1] + aligned_y;
            m--;
        }
    }

    save_pair_into_file(file_path, make_pair(aligned_x, aligned_y));   
    cout << "Needleman-wunsch score: " << ret << endl;
}
