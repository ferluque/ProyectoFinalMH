#include "Solution.h"
#include "Problem.h"
#include "random.hpp"
#include <vector>

using namespace std;
using Random = effolkronium::random_static;

int main () {
    // Generamos la población inicial de forma aleatoria
    Problem p("..\\datos\\GKD-b_21_n100_m10.txt");
    int M = 60;
    vector<Solution> inicial = GeneraPoblacion(M,p);
    SolutionCompare comp;
    sort(inicial.begin(), inicial.end(),comp);
    cout << inicial.size() << endl;
    int evaluaciones = 0;

    while (evaluaciones < 1e5) {
        // Dividimos en machos y hembras
        // Las hembras serán la primera mitad y los machos la última
        vector<Solution> hembras = vector<Solution>(inicial.begin(), inicial.begin() + M / 2);
        vector<Solution> machos = vector<Solution>(inicial.begin() + M / 2, inicial.end());
//        cout << "Hembras: " << endl << hembras << endl;
//        cout << "Machos: " << endl << machos << endl;

        // Dividimos de nuevo en los subgrupos
        vector<Solution> parthenogenesis = vector<Solution>(hembras.begin(), hembras.begin() + hembras.size() / 2);
        vector<Solution> polyandry = vector<Solution>(hembras.begin() + hembras.size() / 2, hembras.end());

        vector<Solution> monogamy = vector<Solution>(machos.begin(), machos.begin() + machos.size() / 3);
        vector<Solution> polygyny = vector<Solution>(machos.begin() + machos.size() / 3,
                                                     machos.begin() + machos.size() * 2 / 3);

        // Antes de comenzar a hacer cruces, hay que asignar probabilidad a los pájaros de cruzarse, a mejor valor
        // de diff (menor) mayor probabilidad de cruzarse.
        // La probabilidad de cruzarse de una solución se calculará como:
        // 1-(p.get_diff())/(maximo get_diff()) or tanto, la mejor hembra, tendrá mayor probabilidad de generar
        // una cría que el peor macho (que no generará nunca)
        vector<float> probabilidades(M);
        float maximo_diff = machos[machos.size()-1].get_diff();
        for (int i=0; i<probabilidades.size(); i++) {
            probabilidades[i] = 1-inicial[i].get_diff()/maximo_diff;
        }

        inicial.clear();
//        cout << "Probabilidades: " << endl << probabilidades << endl;
        // HACEMOS LOS CRUCES QUE CORRESPONDAN

        // NO cruces de parthenogenesis, unicamente mutación
        // Se avanza en las hembras de parthenogenesis decidiendo por su probabilidad si estas mutan hasta
        //  que se alcance un límite (el 50% de las de este grupo) (normalmente
        // solo mutarán las mejores, es lo que se busca en esta parte de la población)
        int MaxParth=0.5*parthenogenesis.size();
        int mutadas = 0;
//        cout << "Parthenogenesis antes: " << parthenogenesis<< endl;
        for (int i = 0; (i < parthenogenesis.size()) && (mutadas < MaxParth); i++) {
            if (Random::get<float>(0, 1) < probabilidades[i]) {
                Solution cria = parthenogenesis[i].mutacion(p);
                evaluaciones++;
                if (cria.get_diff() < parthenogenesis[i].get_diff())
                    parthenogenesis[i] = cria;
            }
        }
//        cout <<"Parthenogenesis después: " << parthenogenesis << endl;

        // Polyandry, una hembra con varios machos

        // Monogamy, un macho y una hembra aleatoria (los machos monógamos todos tienen la misma probabilidad de cruzarse
        // pero lo harán más frecuentemente con las mejores hembras hasta alcanzar un máximo de cruces, el 50% de los machos
        // monógamos
        vector<int> machos_cruzados = range(0,monogamy.size());
        Random::shuffle(machos_cruzados);
        int cruces_monogamy = 0.5*monogamy.size();
//        cout << "Monogamy antes: " << monogamy << endl;
        for (int i = 0; i < cruces_monogamy; i++) {
            Solution macho = monogamy[machos_cruzados[i]];
            vector<int> hembras_posibles = range(0, hembras.size());
            Random::shuffle(hembras_posibles);
            bool cruzado = false;
            int j = 0;
            while (!cruzado) {
                if (Random::get<float>(0, 1) < probabilidades[hembras_posibles[j]]) {
                    Solution cria = macho.cruce_posicion(hembras[hembras_posibles[j]], p).first;
                    evaluaciones++;
                    if (cria.get_diff() < macho.get_diff())
                        monogamy[machos_cruzados[i]] = cria;
                    cruzado = true;
                }
            }
        }
//        cout << "Monogamy después: " << monogamy << endl;

        // Polygyny



        // UNA VEZ AVANZADA LA GENERACIÓN, VOLVEMOS A UNIR Y VOLVEMOS A ORDENAR, AHORA MACHOS PUEDEN
        // PASAR A SER HEMBRAS
        inicial.insert(inicial.begin(), parthenogenesis.begin(), parthenogenesis.end());
        inicial.insert(inicial.end(), polyandry.begin(), polyandry.end());
        inicial.insert(inicial.end(), monogamy.begin(), monogamy.end());
        inicial.insert(inicial.end(), polygyny.begin(), polygyny.end());
        vector<Solution> promiscuous = GeneraPoblacion(polygyny.size(), p);
        evaluaciones+=polygyny.size();
        inicial.insert(inicial.end(), promiscuous.begin(), promiscuous.end());

        sort(inicial.begin(), inicial.end(), comp);
    }
    cout << inicial[0] << endl;
}