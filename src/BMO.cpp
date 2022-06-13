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
    int evaluaciones = M;
    SolutionCompare comp;
    sort(inicial.begin(), inicial.end(),comp);
    cout << inicial.size() << endl;
    int cambio = 0;
    bool cambiado = true;
    while ((evaluaciones < 1e5)&&(cambio<2)) {
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
                if (cria.get_diff() < parthenogenesis[i].get_diff()) {
                    parthenogenesis[i] = cria;
                    cambiado = true;
                }
            }
        }
//        cout <<"Parthenogenesis después: " << parthenogenesis << endl;

        // Polyandry, una hembra con varios machos
        // Realmente el operador es el mismo que para polygyny
        // Ahora, las hembras poliandricas cruzarán con machos al azar, sin tener en cuenta si son mejores o peores
        int MaxPolyandry=0.3*polyandry.size();
        vector<int> hembras_seleccionadas = range(0,polyandry.size());
        Random::shuffle(hembras_seleccionadas);
        for (int i=0; i<MaxPolyandry; i++){
            int num_padres = 4;
            vector<int> padres = range(0,machos.size());
            Random::shuffle(padres);
            vector<Solution> padres_s(num_padres);
            for (int i=0; i<num_padres;i++)
                padres_s[i] = machos[padres[i]];
            Solution cria = polyandry[hembras_seleccionadas[i]].cruce_multiple(padres_s,p);
            evaluaciones++;
            if (cria.get_diff() < polyandry[hembras_seleccionadas[i]].get_diff()) {
                polyandry[hembras_seleccionadas[i]] = cria;
                cambiado = true;
            }
        }

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
                    Solution cria = macho.cruce_posicion(hembras[hembras_posibles[j]], p);
                    evaluaciones++;
                    if (cria.get_diff() < macho.get_diff()) {
                        monogamy[machos_cruzados[i]] = cria;
                        cambiado = true;
                    }
                    cruzado = true;
                }
            }
        }
//        cout << "Monogamy después: " << monogamy << endl;

        // Polygyny
        // Igual que polyandry pero para los machos, igualmente, las hembras se cogen de forma aleatoria, no se
        // tiene en cuenta si son mejores o peores.
        int MaxPolygyny=0.3*polygyny.size();
        vector<int> machos_seleccionados = range(0,polygyny.size());
        Random::shuffle(machos_seleccionados);
        for (int i=0; i<MaxPolygyny; i++){
            int num_madres = 4;
            vector<int> madres = range(0,polygyny.size());
            Random::shuffle(madres);
            vector<Solution> madres_s(num_madres);
            for (int i=0; i<num_madres;i++)
                madres_s[i] = hembras[madres[i]];
            Solution cria = polygyny[machos_seleccionados[i]].cruce_multiple(madres_s,p);
            evaluaciones++;
            if (cria.get_diff() < polygyny[machos_seleccionados[i]].get_diff()) {
                polygyny[machos_seleccionados[i]] = cria;
                cambiado = true;
            }
        }

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
        if (cambiado)
            cambio = 0;
        else
            cambio++;
    }
    cout << inicial[0] << endl;
}