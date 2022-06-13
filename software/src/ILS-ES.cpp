//
// Created by Fernando on 21/05/2022.
//

#include "bl.h"
#include "Solution.h"
#include "random.hpp"
#include <filesystem>

using namespace std;

using Random = effolkronium::random_static;

Solution ES_mod(Problem p, Solution s0);
Solution ILS_ES(Problem p);

int main () {
    Random::seed(0);

    string path = "..\\datos";
    ofstream salida;
    salida.open("..\\resultados\\ILS-ES.csv");
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        string file = entry.path().u8string();
        salida << file << ";";

        Problem p(file);
        auto tinicio = clock();
        Solution S = ILS_ES(p);
        auto tfin = clock();
        salida << S.get_diff() << ";" << double(tfin-tinicio)/CLOCKS_PER_SEC*1000 << ";" << endl;
        cout << "Terminado " << file << endl;
    }


}

Solution ILS_ES(Problem p) {
    Solution mejor;
    Solution s = SolucionAleatoria(p);
    s = bl(p,s);
    mejor = s;

    for (int i=1; i<10;i++) {
        s = ES_mod(p,mejor.mutacion(p));
        if (mejor.get_diff()>s.get_diff())
            mejor = s;
    }
    return mejor;
}

Solution ES_mod(Problem p, Solution s0) {
    Solution s = s0;
    for (int i=0; i<s.get_selected().size();i++)
        p.extract(s.get_selected()[i]);
    // Calculamos la temperatura inicial
    float mu = 0.3, phi = 0.3;
    float T0 = (mu*s.get_diff())/(-log(phi));
    // Y la final, comprobando que sea menor que la inicial
    float Tf = 1e-3;
    if (T0<Tf)
        Tf = T0/100;

    // Números máximos
    int max_vecinos = 10*p.get_n();
    int vecinos_generados = 0;
    int max_exitos = 0.1*max_vecinos;
    int exitos = 0;
    int max_evaluaciones = 1e4;
    int M = max_evaluaciones/max_vecinos;
    float beta = (T0-Tf)/(M*T0*Tf);
    bool para = false;

    // Algoritmo
    Solution mejor(s);
    int evaluaciones = 0;
    float T = T0;
    while ((T>Tf)&&(evaluaciones < max_evaluaciones)&&(!para)) {
        vector<pair<int,int>> vecinos = vecinosPosibles(s,p);
        Random::shuffle(vecinos);
        for (auto it=vecinos.begin();(it!=vecinos.end())&&(!para);it++) {
            int saca = (*it).first, mete = (*it).second;
            Solution vecina = s.neighbor(saca,mete,p.get_d());
            vecinos_generados++;
            evaluaciones +=1;

            float dist_diff = abs(s.get_diff()-vecina.get_diff());
            if ((vecina.get_diff()<s.get_diff()) || (Random::get<float>(0,1)<=exp(-dist_diff/T))) {
                s = vecina;
                p.insert(saca);
                p.extract(mete);

                if (s.get_diff()<mejor.get_diff())
                    mejor = s;
                exitos++;
                break;
            }
            evaluaciones += 3;
            if (vecinos_generados >= max_vecinos) {
                T = T/(1+beta*T);
                vecinos_generados = 0;
                if (exitos == 0)
                    para = true;
                else
                    exitos = 0;
            }
            if (exitos >= max_exitos) {
                T = T/(1+beta*T);
                exitos = 0;
                vecinos_generados = 0;
            }
        }
    }
    return mejor;
}
