#include "needleman_wunsch.h"

using namespace std;

void needleman_wnsch(string x, string y){ 
    int n = x.length();
    int m = y.length();

    vector<vector<int>> matrix (n + 1, vector<int>(m + 1, 0));
    int match = 1, mismatch = -1, gap = -2;

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

    while(n > 0 || m > 0){
        if(n > 0 
        && m > 0 
        && (matrix[n][m] == matrix[n-1][m-1] + match 
        || matrix[n][m] == matrix[n-1][m-1] + mismatch)){
            aligned_x[n] += x[n-1];
            aligned_y[n] += y[m-1];
            n--;
            m--;
        } else if(n > 0 && matrix[n][m] == matrix[n-1][m] + gap){
            aligned_x += x[n-1];
            aligned_y += '-';
            n--;
        } else {
            aligned_x += '-';
            aligned_y += y[m-1];
            m--;
        }
    }

    // Reverse the aligned strings
    std::reverse(aligned_x.begin(), aligned_x.end());
    std::reverse(aligned_y.begin(), aligned_y.end());

    // Output the alignment
    std::cout << "Aligned Sequence X: " << aligned_x << std::endl;
    std::cout << "Aligned Sequence Y: " << aligned_y << std::endl;
}
