//
// Created by Fernando on 08/05/2022.
//
#include "Solution.h"
#include <algorithm>
#include "random.hpp"

using Random = effolkronium::random_static;
using namespace std;

void Solution::check_max_min(float new_delta) {
    max_delta = min_delta = deltas.begin()->second;
    for (auto it = deltas.begin(); it != deltas.end(); ++it) {
        if ((*it).second > max_delta)
            max_delta = (*it).second;
        if ((*it).second < min_delta)
            min_delta = (*it).second;
    }
    diff = max_delta - min_delta;
}

float Solution::delta(int u, const std::vector<std::vector<float>> &d) const {
    float de = 0.0;
    for (int i: selected)
        de += d[i][u];
    return de;
}

Solution Solution::exchange(int i, int j, vector<vector<float>> d) {
    // Saco la i de selected y de deltas
    selected.erase(find(selected.begin(), selected.end(), i));
    deltas.erase(i);

    // Meto j en selected
    selected.push_back(j);
    deltas[j] = delta(j,d);

    // FUNCIÓN OBJETIVO FACTORIZADA
    // Actualizo los valores de min_delta y max_delta
    // Para ello tengo que restar d[i][selected[k]] y sumar d[j][selected[k]] a los deltas
    deltas[selected[0]] = min_delta = max_delta = deltas[selected[0]] - d[i][selected[0]] + d[j][selected[0]];
    for (int k = 1; k < selected.size()-1; k++) {
        float de = deltas[selected[k]] = deltas[selected[k]] - d[i][selected[k]] + d[j][selected[k]];
        if (de>max_delta)
            max_delta = de;
        if (de<min_delta)
            min_delta = de;
    }
    if (deltas[j]>max_delta)
        max_delta = deltas[j];
    if (deltas[j]<min_delta)
        min_delta = deltas[j];
    diff = max_delta - min_delta;
    return *this;
}

Solution::Solution(std::vector<int> s, const Problem& p) {
    selected = s;
    deltas[selected[0]] = max_delta = min_delta = delta(s[0], p.get_d());
    for (int i = 1; i < selected.size(); i++) {
        float de = delta(selected[i],p.get_d());
        deltas[selected[i]] = de;
        if (de>max_delta)
            max_delta = de;
        if (de<min_delta)
            min_delta = de;
    }
    diff = max_delta - min_delta;
}

Solution::Solution(const Solution &c) {
    this->selected = c.selected;
    this->max_delta = c.max_delta;
    this->min_delta = c.min_delta;
    this->diff = c.diff;
    this->deltas = c.deltas;
};

Solution Solution::neighbor(int i, int j, std::vector<std::vector<float>> d) {
    Solution vecino(*this);
    return vecino.exchange(i, j, d);
}

void Solution::print_dist(std::string file_out, const std::vector<std::vector<float>> &d) {
    ofstream out;
    out.open(file_out);
    out << ";";
    for (int i: selected)
        out << i << ";";
    out << endl;
    for (int i: selected) {
        out << i << ";";
        for (int j: selected) {
            out << d[i][j] << ";";
        }
        out << endl;
    }
}

Solution::Solution(){}

Solution Solution::mutacion(Problem p, float prop) {
    for (int i=0; i<selected.size();i++)
        p.extract(selected[i]);
    int num_cambios = prop*selected.size();
    Solution mutada(*this);
    for (int i=0; i<num_cambios;i++) {
        pair<int,int> intercambio(Random::get<int>(0,mutada.get_selected().size()-1), Random::get<int>(0,p.get_N().size()-1));
        int saca = mutada.get_selected()[intercambio.first], mete=p.get_N()[intercambio.second];
        mutada = mutada.neighbor(saca, mete, p.get_d());
        p.extract(mete);
        p.insert(saca);
    }

    return mutada;
}

ostream& operator<<(ostream& out, Solution s) {
    out << "Escogidos: (";
    vector<int> selected = s.get_selected();
    for (int i : selected) {
        out << i << ", ";
    }
    out << ")" << endl;
    out << "Valor de diff: " << s.get_diff() << endl;
    return out;
}

vector<int> range(int init, int fin) {
    assert(fin>init);
    vector<int> v(fin-init);
    for (int i=init;i<fin;i++)
        v[i-init] = i;
    return v;
}

Solution SolucionAleatoria(Problem& p) {
    vector<int> puntos = range(0,p.get_n());
    Random::shuffle(puntos);
    Solution escogida(vector<int>(puntos.begin(),puntos.begin()+p.get_m()),p);
    return escogida;
}

vector<pair<int,int>> vecinosPosibles(const Solution& s, const Problem& p) {
    vector<pair<int,int>> vecinos;
    for (int i=0; i<s.get_selected().size(); ++i) {
        int saca = s.get_selected()[i];
        for (int j=0; j<p.get_N().size(); ++j) {
            int mete = p.get_N()[j];
            vecinos.push_back(pair<int,int>(saca, mete));
        }
    }
    return vecinos;
}

ostream& operator<<(ostream& out, pair<int,int> p) {
    out << "("<<p.first<< ", "<<p.second << ")";
    return out;
}