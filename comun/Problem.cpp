#include "Problem.h"
#include "Solution.h"
#include <iostream>

using namespace std;

Problem::Problem(string file){
    ifstream input;
    input.open(file);
    if (!input.is_open()) {
        cerr << "Error abriendo el archivo" << endl;
        exit(-1);
    }
    input >> n;
    input >> m;

    d.resize(n);
    for (int i = 0; i < n; i++)
        d[i].resize(n);

    // Leo los datos de la matriz superior
    for (int i = 0; i < (n - 1); i++) {
        for (int j = i + 1; j < n; j++) {
            int i_i, j_i;
            float d_i;
            input >> i_i >> j_i >> d_i;
            d[i][j] = d_i;
            input.ignore();
        }
    }

    // Copio por debajo de la diagonal principal los mismos datos (matriz simétrica)
    // Inicializo con 0s la matriz inferior (i>j)
    for (int i = 0; i < n; i++)
        for (int j = i; j >= 0; j--)
            d[i][j] = d[j][i];
    input.close();

    N.resize(n);
    for (int i=0; i<n; i++)
        N[i] = i;
}

void Problem::extract(int u) {
    N.erase(find(N.begin(), N.end(), u));
}

void Problem::insert(int u) {
    N.push_back(u);
}

int Problem::get_n() const {return n;}

int Problem::get_m() const {return m;}

const vector<int>& Problem::get_N() const {return N;}

const vector<vector<float>>& Problem::get_d() const {return d;}

std::ostream& operator<<(std::ostream& out, Problem p) {
    out << "n: " << p.get_n() << " // m: " << p.get_m() << endl;
    out << "Not selected: " << p.get_N() << endl;
    return out;
}
