//
// Created by Fernando on 21/05/2022.
//

#include <iostream>
#include "random.hpp"
#include "Problem.h"
#include "Solution.h"
#include <filesystem>

using namespace std;

using Random = effolkronium::random_static;

Solution ES(Problem &p);

int main() {
    Random::seed(0);

    string path = "..\\datos";
    ofstream salida;
    salida.open("..\\resultados\\ES.csv");
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        string file = entry.path().u8string();
        salida << file << ";";

        Problem p(file);
        auto tinicio = clock();
        Solution S = ES(p);
        auto tfin = clock();
        salida << S.get_diff() << ";" << double(tfin-tinicio)/CLOCKS_PER_SEC*1000 << ";" << endl;
        cout << "Terminado " << file << endl;
    }
//    Problem p("..\\datos\\GKD-b_6_n25_m7.txt");
//    Solution S = ES(p);
//    cout << S;

}

Solution ES(Problem &p) {
    Solution s = SolucionAleatoria(p);
    for (int i = 0; i < s.get_selected().size(); i++)
        p.extract(s.get_selected()[i]);
    // Calculamos la temperatura inicial
    float mu = 0.3, phi = 0.3;
    float T0 = (mu * s.get_diff()) / (-log(phi));
    // Y la final, comprobando que sea menor que la inicial
    float Tf = 1e-3;
    if (T0 < Tf)
        Tf = T0 / 100;

    // Números máximos
    int max_vecinos = 10 * p.get_n();
    int vecinos_generados = 0;
    int max_exitos = 0.1 * max_vecinos;
    int exitos = 0;
    int max_evaluaciones = 1e5;
    int M = max_evaluaciones / max_vecinos;
    float beta = (T0 - Tf) / (M * T0 * Tf);
    bool para = false;

    // Algoritmo
    Solution mejor(s);
    int evaluaciones = 0;
    float T = T0;
    while ((T > Tf) && (evaluaciones < max_evaluaciones)&&(!para)) {
        // Generamos vecinos
        vector<pair<int, int>> vecinos = vecinosPosibles(s, p);
        Random::shuffle(vecinos);
        // Los recorremos aleatoriamente
        for (auto it = vecinos.begin(); (it != vecinos.end())&&(!para); it++) {
            int saca = (*it).first, mete = (*it).second;
            // Generamos un vecino
            // Incremento las evaluaciones en 1 porque genero un vecino, lo cual supone un recálculo del fitness
            Solution vecina = s.neighbor(saca, mete, p.get_d());
            evaluaciones++;
            vecinos_generados++;
            // Se acepta si es mejor o si la probabilidad de la temperatura lo permite
            float dist_diff = vecina.get_diff() - s.get_diff();
            if ((dist_diff < 0) || (Random::get<float>(0, 1) <= exp(-dist_diff / T))) {
                s = vecina;
                p.insert(saca);
                p.extract(mete);

                if (s.get_diff() < mejor.get_diff())
                    mejor = s;
                exitos++;
                break;
            }

            // Condición de enfriamiento
            if (vecinos_generados >= max_vecinos) {
                T = T / (1 + beta * T);
                vecinos_generados = 0;
                // Condición de parada
                // Si no ha habido éxitos en este enfriamiento
                if (exitos == 0)
                    para = true;
                else
                    exitos = 0;
            }
            if (exitos >= max_exitos) {
                T = T / (1 + beta * T);
                exitos = 0;
                vecinos_generados = 0;
            }
        }
    }
//    cout << "Temperatura final: " << T << endl;
//    cout << "Evaluaciones: " << evaluaciones << endl;
    return mejor;
}