//
// Created by fl156 on 29/03/2022.
//

#ifndef MDD_SOLUTION_H
#define MDD_SOLUTION_H

#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include "random.hpp"
#include "Problem.h"


class Solution {
private:
    std::vector<bool> selected;
    float diff;
    float max_delta;
    float min_delta;
    std::map<int,float> deltas;

    void check_max_min();

    /**
     * Calcula la dispersión de un punto u en el conjunto de seleccionados
     * @param u El punto
     * @param d La matriz de distancias
     * @return El valor de delta
     */
    float delta(int u, const std::vector<std::vector<float>>& d) const;

    float avg(std::vector<bool> h, const Problem& p) const;

public:
    Solution();
    /**
     * Constructor de copia
     * @param c
     */
    Solution(const Solution& c);

    float get_diff() const;

    int get_size() const;

    const std::vector<bool>& get_selected();

    void print_dist(std::string file_out, const std::vector<std::vector<float>>& d);

    // AGG-Uniforme
    std::pair<Solution,Solution> cruce_uniforme(const Solution& s, const Problem& p) const;
    std::vector<bool> repare(std::vector<bool> h, const Problem& p) const;
    Solution(const std::vector<bool>& s, const Problem& p);

    // AGG-posicion
    std::pair<Solution,Solution> cruce_posicion(const Solution& s, const Problem& p) const;

    // AGG común
    Solution mutacion(const Problem& p) const;
    Solution(int size);
};

std::ostream& operator<<(std::ostream& out, Solution s);

template <class T>
std::ostream& operator<<(std::ostream& out, std::vector<T> v) {
    out << "(";
    for (auto it=v.begin(); it!=v.end();++it)
        out << *it << ",";
    out <<")" << std::endl;
    return out;
}

class SolutionCompare {
public:
    bool operator()(const Solution& s1, const Solution& s2) {
        return s1.get_diff()<s2.get_diff();
    };
};

std::vector<int> range(int init, int fin);

std::vector<Solution> GeneraPoblacion(int M, Problem problema);


#endif //MDD_SOLUTION_H
