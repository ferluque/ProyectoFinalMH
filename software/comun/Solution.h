//
// Created by fl156 on 29/03/2022.
//

#ifndef MDD_SOLUTION_H
#define MDD_SOLUTION_H

#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include "Problem.h"

class Solution{
private:
    std::vector<int> selected;
    float diff;
    float max_delta;
    float min_delta;
    std::map<int,float> deltas;

    void check_max_min(float new_delta);

    /**
     * Calcula la dispersión de un punto u en el conjunto de seleccionados
     * @param u El punto
     * @param d La matriz de distancias
     * @return El valor de delta
     */
    float delta(int u, const std::vector<std::vector<float>>& d) const;

    /**
     * Intercambia el vecino i por j y modifica valores correspondientes
     * @param i
     * @param j
     * @param d
     * @return Devuelve la solución con los puntos cambiados
     */
    Solution exchange(int i, int j, std::vector<std::vector<float>> d);
public:
    /**
     * Constructor que inicializa la solución al vector de seleccionados que le pasamos (función utilizada en la bl)
     * @param s El vector de seleccionados
     * @param d La matriz de distancias
     */
    Solution(std::vector<int> s, const Problem& p);

    /**
     * Constructor de copia
     * @param c
     */
    Solution(const Solution& c);
    /**
     * Mecanismo de generación de vecinos. Intercambia el elemento i por el j y devuelve la solución con el respectivo
     * valor de diff, max y min delta y deltas
     * @param i El elemento que saca
     * @param j El elemento que mete
     * @return El vecino
     */
    Solution neighbor(int i, int j, std::vector<std::vector<float>> d);


    float get_diff() const {return diff;};

    int get_size() const {return (int)selected.size();};

    const std::vector<int>& get_selected() const {return selected;};

    void print_dist(std::string file_out, const std::vector<std::vector<float>>& d);

    Solution();

    Solution mutacion(Problem p, float prop=0.3);
};

std::ostream& operator<<(std::ostream& out, Solution s);

std::ostream& operator<<(std::ostream& out, std::pair<int,int> p);

template <class T>
std::ostream& operator<<(std::ostream& out, std::vector<T> v) {
    out << "(";
    for (auto it=v.begin(); it!=v.end();++it)
        out << *it << ",";
    out <<")" << std::endl;
    return out;
}

std::vector<int> range(int init, int fin);

std::vector<Solution> genera_p0(int M, const Problem& p);

Solution SolucionAleatoria(Problem& p);

std::vector<std::pair<int,int>> vecinosPosibles(const Solution& s, const Problem& p);

#endif //MDD_SOLUTION_H
