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

template<class T>
void print_mat(vector<vector<T>> mat) {
    for (auto row : mat) {
        for (auto x : row) {
            cout << x << ' ';
        }
        cout << endl;
    }
}

// We assume that m rows in mat already exist, just empty rows though.
// Initalize all elements to 0.
template<class T>
void init_mat(vector<vector<T>>& mat, int n) {
    for (auto& row : mat) {
        for (int i = 0; i < n; i++) {
            row.push_back(0);
        }
    }
}

template<class T>
vector<vector<T>> transpose(const vector<vector<T>>& A) {
    int m = A.size();
    int n = A[0].size();
    if (m != n) throw invalid_argument("Impossible to transpose a non-square matrix.");
    vector<vector<T>> A_transpose(m);
    init_mat(A_transpose, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A_transpose[i][j] = A[j][i];
        }
    }
    return A_transpose;
}

vector<vector<double>> gen_dct_mat(const int n) {
    vector<vector<double>> C(n);
    init_mat(C, n);

    /*
        DCT matrix element definition:
            C[i][j] = a*cos(((2j + 1)*i*pi) / (2*N)), for i, j = 0, ..., N - 1
            a = sqrt(1/N) for i = 0
            a = sqrt(2/N) for i = 1, ..., N - 1
    */
    const double a0 = sqrt(1.0/n);
    const double ai = sqrt(2.0/n);

    double a;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a = (i == 0) ? a0 : ai;
            C[i][j] = a*cos(((2*j + 1)*i*M_PI) / (2*n));
        }
    }

    return C;
}

vector<vector<double>> multiply(const vector<vector<double>>& A, const vector<vector<double>>& B) {
    auto m = A.size();
    auto n = A[0].size(); // Assume all rows are the same length.
    if (n != B.size()) throw invalid_argument("Number of columns in A must match number of rows in B.");
    auto p = B[0].size();

    vector<vector<double>> AB(m); // m x p matrix = AB
    init_mat(AB, p);

    // Multiply.
    for (vector<vector<double>>::size_type i = 0; i < m; i++) {
        for (vector<vector<double>>::size_type j = 0; j < p; j++) {
            for (vector<vector<double>>::size_type k = 0; k < n; k++) {
                AB[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    return AB;
}

void round(vector<vector<double>>& A) {
    for (auto& row : A) {
        for (auto& x : row) {
            x = round(x);
        }
    }
}
template<class T>
auto dct(const vector<vector<double>>& C, const vector<vector<double>>& C_transpose, vector<vector<T>> A, 
        bool rows_first) {
    if (rows_first) {
        auto temp = multiply(C, multiply(A, C_transpose));
        round(temp);
        return temp;
    }
    else {
        auto temp = multiply(multiply(C, A), C_transpose);
        round(temp);
        return temp;
    }
}

template<class T>
bool equals(const vector<vector<T>>& A, const vector<vector<T>>& B) {
    int m = A.size();
    int n = A[0].size();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (A[i][j] != B[i][j]) {
                return false;
            }
        }
    }
    return true;
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
    vector<vector<double>> A(N);
    for (int k = 0; getline(file, line) && !all_of(line.begin(), line.end(), isspace); k++) {
        stringstream ss(line);
        string elem;
        for (int i = 0; i < N; i++) {
            getline(ss, elem, ' ');
            A[k].push_back(stoi(elem));
        }
    }

    vector<vector<double>> C = gen_dct_mat(N);
    vector<vector<double>> C_transpose = transpose(C);

    // Y = TXTt
    cout << "Result of transforming rows first, then columns:" << endl;
    // For rows first, Y = T(XTt)
    vector<vector<double>> Y_rows_first = dct(C, C_transpose, A, true);// multiply(C, multiply(A, C_transpose));
    print_mat(Y_rows_first);
    cout << endl;

    cout << "Result of transforming columns first, then rows:" << endl;
    // For columns first, Y = (TX)Tt
    vector<vector<double>> Y_columns_first = dct(C, C_transpose, A, false);// multiply(multiply(C, A), C_transpose);
    print_mat(Y_columns_first);
    cout << endl;

    if (equals(Y_rows_first, Y_columns_first)) {
        cout << "The two matrices are equal." << endl;
    }
    else {
        cout << "The two matrices are not equal." << endl;
    }
    

    return 0;
}
