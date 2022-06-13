//
// Created by Fernando on 30/03/2022.
//

#ifndef MDD_PROBLEM_H
#define MDD_PROBLEM_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

class Problem {
private:
    int n,m;
    std::vector<std::vector<float>> d;
    std::vector<int> N;

public:
    /**
     * Constructor que inicializa los valores de n,m y d a razón de lo que haya en el fichero de parámetro
     * @param file El fichero del que extraerlos
     */
    Problem(std::string file);

    /**
     * Extrae un punto de la lista de restantes
     * @param u El punto a extraer
     */
    void extract(int u);

    void insert(int u);

    int get_n() const;
    int get_m() const;
    const std::vector<int>& get_N() const;
    const std::vector<std::vector<float>>& get_d() const;
};

std::ostream& operator<<(std::ostream& out, Problem p);

#endif //MDD_PROBLEM_H
