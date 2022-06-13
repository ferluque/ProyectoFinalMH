//
// Created by Fernando on 21/05/2022.
//

#include "bl.h"
#include "Solution.h"
#include "random.hpp"
#include <filesystem>

using namespace std;

using Random = effolkronium::random_static;

Solution ILS(Problem p);

int main () {
    Random::seed(0);

    string path = "..\\datos";
    ofstream salida;
    salida.open("..\\resultados\\ILS.csv");
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        string file = entry.path().u8string();
        salida << file << ";";

        Problem p(file);
        auto tinicio = clock();
        Solution S = ILS(p);
        auto tfin = clock();
        salida << S.get_diff() << ";" << double(tfin-tinicio)/CLOCKS_PER_SEC*1000 << ";" << endl;
        cout << "Terminado " << file << endl;
    }
}

Solution ILS (Problem p) {
    Solution mejor;
    Solution s = SolucionAleatoria(p);
    s = bl(p,s);
    mejor = s;

    for (int i=1; i<10;i++) {
        s = bl(p,mejor.mutacion(p));
        if (mejor.get_diff()>s.get_diff())
            mejor = s;
    }
    return mejor;
}