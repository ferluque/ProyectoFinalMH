//
// Created by Fernando on 08/05/2022.
//

#include "bl.h"
#include "random.hpp"
#include "Solution.h"

using namespace std;
using Random = effolkronium::random_static;

Solution bl(Problem p, Solution s) {
    int contador = 0;

    int evaluaciones = 0;

//    cout << "Solución antes de bl " << s << endl;
//    s.print_dist("..\\resultados\\debugging\\antesbl.csv",p.get_d());

    for (int i=0; i<s.get_selected().size();i++)
        p.extract(s.get_selected()[i]);
    // Genero vecinos hasta encontrar el primer mejor
    // Puedo cambiar m posiciones
    // cada una con n-m elementos posibles

    // MODIFICACIÓN: Ahora estos vecinos no se exploran directamente sino que se almacenan en un vector
    // se baraja y se van explorando en el orden que nos de el aleatorio
    while (contador < 1e4) {
        bool vecindario_explorado = true;
        // También almacenamos el cambio que supone la generación de ese vecino
        vector<pair<int,int>> vecinos;
        for (int i=0; (i<s.get_selected().size())&&(contador < 1e5); ++i) {
            int saca = s.get_selected()[i];
            for (int j=0; (j<p.get_N().size())&&(contador < 1e5); ++j) {
                int mete = p.get_N()[j];
                vecinos.push_back(pair<int,int>(saca, mete));
            }
        }
        // Una vez genere todos los vecinos los barajamos y los vamos sacando
        Random::shuffle(vecinos);
        auto inicioexploracion = clock();
        for (auto it = vecinos.begin(); it!=vecinos.end(); ++it) {
            ++contador;
            if (contador == 1e5)
                break;

            Solution v = s.neighbor((*it).first, (*it).second, p.get_d());

            if (v.get_diff() < s.get_diff()) {
                s = v;

                p.insert((*it).first);
                p.extract((*it).second);
                vecindario_explorado = false;
                break;
            }
            evaluaciones += 2;
        }
        if (vecindario_explorado)
            break;
    }

    return s;
}