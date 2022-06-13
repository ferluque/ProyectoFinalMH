//
// Created by Fernando on 21/05/2022.
//

#include "bl.h"
#include "Solution.h"
#include "random.hpp"
#include <filesystem>

using namespace std;
using Random = effolkronium::random_static;

Solution BMB(Problem p);

int main () {
    Random::seed(0);
    int M=10;

    string path = "..\\datos";
    ofstream salida;
    salida.open("..\\resultados\\BMB.csv");
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        string file = entry.path().u8string();
        salida << file << ";";

        Problem p(file);
        auto tinicio = clock();
        Solution S = BMB(p);
        auto tfin = clock();
        salida << S.get_diff() << ";" << double(tfin-tinicio)/CLOCKS_PER_SEC*1000 << ";" << endl;
        cout << "Terminado " << file << endl;
    }
}

Solution BMB(Problem p) {
    Solution mejor;
    for (int i=0; i<10;i++) {
        Problem pi(p);
        Solution s = SolucionAleatoria(pi);
        // Los puntos en s se extraen de p en bl
        s = bl(pi,s);
        if ((i==0) || (mejor.get_diff()>s.get_diff()))
            mejor = s;
    }
    return mejor;
}