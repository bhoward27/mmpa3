/*
    Assignment Description:

    2D (Discrete Cosine Transform) 
    
    Read N (an integer between 2 and 16) and then an input matrix of size NxN from an input file.
    Each matrix component is an integer between 0 and 255. Print out the 2D DCT transform result (rounding to integers) with
        (1) rows transform first and then columns, and
        (2) columns transform first and then rows.
    
    Note that you can't call any lib to do this; you have to write your own code to implement the matrix operation.
    Discuss whether results (1) and (2) are identical. If not, what's the potential problems.


    David's email:

    The first row is the integer N, indicating the size of NxN matrix. Each of the subsequent lines represents a row of the NxN 
    matrix from the top row to the bottom row. Each input file will contain only one matrix.

    I will expect your program to read one argument for the path of the input file. For example, suppose your program is named as 
    app.py, then your program should be able to run with the following command:

        python app.py /home/my_assignment/pa3_sample.txt

    If your program works in a slightly different way to read the input file, that is ok. 
    Please make sure you document it in your report.

    DCT matrix element definition:
        C[i][j] = a*cos((2j + 1)*i*pi) / (2*N)), for i, j = 0, ..., N - 1
        a = sqrt(1/N) for i = 0
        a = sqrt(2/N) for i = 1, ..., N - 1
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <stdexcept>

using std::cin;
using std::cout;
using std::vector;
using std::endl;
using std::string;
using std::ifstream;
using std::all_of;
using std::stringstream;
using std::invalid_argument;

void print_mat(vector<vector<int>> mat) {
    for (auto row : mat) {
        for (int x : row) {
            cout << x << ' ';
        }
        cout << endl;
    }
}

vector<vector<int>> multiply(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int m = A.size();
    int n = A[0].size(); // Assume all rows are the same length.
    if (n != B.size()) throw invalid_argument("Number of columns in A must match number of rows in B.");
    int p = B[0].size();

    vector<vector<int>> AB(m); // m x p matrix = AB
    // Initialize all rows in AB to 0.
    for (auto& row : AB) {
        for (int i = 0; i < p; i++) {
            row.push_back(0);
        }
    }

    // Multiply.
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            for (int k = 0; k < n; k++) {
                AB[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    return AB;
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        cout << "Please supply the matrix input file name/path as a command-line argument." << endl;
        return 1;
    }

    string file_name(argv[1]);
    ifstream file;
    file.open(file_name);
    if (!file.is_open()) {
        cout << "Error: File does not exist." << endl;
        return 2;
    }

    string line = "";
    getline(file, line);
    int N = stoi(line);
    vector<vector<int>> mat(N);
    for (int k = 0; getline(file, line) && !all_of(line.begin(), line.end(), isspace); k++) {
        stringstream ss(line);
        string elem;
        for (int i = 0; i < N; i++) {
            getline(ss, elem, ' ');
            mat[k].push_back(stoi(elem));
        }
    }

    cout << "The input matrix is:\n";
    print_mat(mat);
    cout << endl;

    cout << "The input matrix, squared, is:\n";
    print_mat(multiply(mat, mat));
    cout << endl;

    vector<int> x = {1, 2, 3, 4, 5, 6, 7, 8};
    vector<vector<int>> X(8);
    for (int i = 0; i < 8; i++) {
        X[i].push_back(x[i]);
    }
    cout << "Ax = \n";
    print_mat(multiply(mat, X));


    return 0;
}
