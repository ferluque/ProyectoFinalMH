//
// Created by fl156 on 05/05/2022.
//

#include "Solution.h"
#include <map>

using namespace std;
using Random = effolkronium::random_static;

void Solution::check_max_min() {
    max_delta = min_delta = deltas.begin()->second;
    for (auto it = deltas.begin(); it != deltas.end(); ++it) {
        if ((*it).second > max_delta)
            max_delta = (*it).second;
        if ((*it).second < min_delta)
            min_delta = (*it).second;
    }
    diff = max_delta - min_delta;
}

float Solution::delta(int u, const vector<vector<float>> &d) const {
    float de = 0.0;
    for (int i = 0; i < selected.size(); i++)
        if (selected[i])
            de += d[i][u];
    return de;
}

Solution::Solution(const Solution &c) {
    this->selected = c.selected;
    this->max_delta = c.max_delta;
    this->min_delta = c.min_delta;
    this->diff = c.diff;
    this->deltas = c.deltas;
}

float Solution::get_diff() const { return diff; }

int Solution::get_size() const { return (int) selected.size(); }

const vector<bool> &Solution::get_selected() { return selected; };

void Solution::print_dist(string file_out, const vector<vector<float>> &d) {
    ofstream out;
    out.open(file_out);
    out << ";";
    for (int i = 0; i < selected.size(); i++)
        if (selected[i])
            out << i << ";";
    out << endl;
    for (int i = 0; i < selected.size(); i++) {
        if (selected[i]) {
            out << i << ";";
            for (int j = 0; j < selected.size(); j++) {
                if (selected[j])
                    out << d[i][j] << ";";
            }
            out << endl;
        }
    }
}

ostream &operator<<(ostream &out, Solution s) {
    out << "Escogidos: (";
    vector<bool> selected = s.get_selected();
    for (int i = 0; i < selected.size(); i++)
        if (selected[i])
            out << i << ", ";
    out << ")" << endl;
    out << "Valor de diff: " << s.get_diff() << endl;
    return out;
}

// AGG-Uniforme
// Depurar
Solution Solution::cruce_uniforme(const Solution &s, const Problem &p) const {
    vector<bool> s1 = this->selected;
    vector<bool> s2 = s.selected;

    vector<bool> h1(s1.size());
    // Nos quedamos con los que coincidan
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] == s2[i])
            h1[i] = s1[i];
            // Los que no se elige aleatoriamente de uno u otro
        else {
            bool uno = Random::get<bool>();
            if (uno)
                h1[i] = s1[i];
            else
                h1[i] = s2[i];
        }
    }
    h1 = repare(h1, p);
    return Solution(h1,p);
}

Solution::Solution(const std::vector<bool> &s, const Problem &p) {
    selected = s;
    for (int i = 0; i < selected.size(); i++) {
        if (selected[i]) {
            deltas[i] = delta(i, p.get_d());
        }
    }
    check_max_min();
}

float Solution::avg(std::vector<bool> h, const Problem& p) const {
    // Calculamos la media
    float avg = 0.0;
    for (int i=0; i<h.size(); i++)
        if (h[i])
            avg += delta(i, p.get_d());
    return avg/(int)std::count(h.begin(), h.end(), true);
}

vector<bool> Solution::repare(vector<bool> h, const Problem &p) const {
    // Calculamos cuánto nos pasamos
    int sizeS = count(h.begin(),h.end(),true);
    int v = p.get_m()-count(h.begin(), h.end(), true);
    if (v==0)
        return h;
    if (v<0) {
        while (v<0) {
            float average = avg(h,p);
            int j;
            bool primero = true;
            float max;
            for (int i=0; i<h.size(); i++) {
                if (h[i]) {
                    if (primero) {
                        j = i;
                        max = abs(delta(j, p.get_d())-average);
                        primero = false;
                    } else {
                        float deltai = delta(i, p.get_d());
                        if (abs(deltai-average) > max) {
                            j = i;
                            max = abs(deltai-average);
                        }
                    }
                }
            }
            h[j] = 0;
            average = avg(h,p);
            v++;
        }
    }
    if (v>0) {
        while (v>0) {
            float average = avg(h,p);
            int j;
            bool primero = true;
            float min;
            for (int i=0; i<h.size(); i++) {
                if (!h[i]) {
                    if (primero) {
                        j = i;
                        min = abs(delta(j, p.get_d())-average);
                        primero = false;
                    } else {
                        float deltai = delta(i, p.get_d());
                        if (abs(deltai-average) < min) {
                            j = i;
                            min = abs(deltai-average);
                        }
                    }
                }
            }
            h[j] = 1;
            average = avg(h,p);
            v--;
        }
    }
    return h;
}

// AGG-posicion
Solution Solution::cruce_posicion(const Solution &s, const Problem &p) const {
    vector<bool> s1 = this->selected;
    vector<bool> s2 = s.selected;

    vector<bool> h1(s1.size());

    vector<bool> restos_p1;
    vector<int> pos;
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] == s2[i])
            h1[i] = s1[i];
        else {
            restos_p1.push_back(s1[i]);
            pos.push_back(i);
        }
    }
    Random::shuffle(restos_p1);
    for (int i = 0; i < restos_p1.size(); i++)
        h1[pos[i]] = restos_p1[i];

    return Solution(h1, p);
}

Solution Solution::mutacion(const Problem &p) const {
    Solution mutada(*this);
    // Generamos los genes que van a mutar
    // Hay que hacer que necesariamente se cambie un 0 por un 1 o viceversa
    int xi = Random::get<int>(0, selected.size() - 1), xj = Random::get<int>(0, selected.size() - 1);
    while (selected[xi] == selected[xj]) {
        xi = Random::get<int>(0, selected.size() - 1);
        xj = Random::get<int>(0, selected.size() - 1);
    }
    bool aux = mutada.selected[xi];
    mutada.selected[xi] = mutada.selected[xj];
    mutada.selected[xj] = aux;
    // Si ahora es el xi el que pasa a ser 1 (antes xj era 1)
    if (mutada.selected[xi]) {
        // Eliminamos el xj de los deltas
        // Antes de eliminar el xj de los deltas, debemos restarlo a los otros deltas
        for (auto it = mutada.deltas.begin(); it != mutada.deltas.end(); ++it)
            if ((*it).first != xj)
                (*it).second -= p.get_d()[xj][(*it).first];
        mutada.deltas.erase(xj);

        // Ahora ya añadimos el xi
        mutada.deltas[xi] = mutada.delta(xi, p.get_d());
        // Y sumamos el delta que añade al resto
        for (auto it = mutada.deltas.begin(); it != mutada.deltas.end(); ++it)
            if ((*it).first != xi)
                (*it).second += p.get_d()[xi][(*it).first];
    }
        // Si es el xj el que pasa a ser 1
    else {
        // Eliminamos el xi de los deltas
        // Antes de eliminar el xj de los deltas, debemos restarlo a los otros deltas
        for (auto it = mutada.deltas.begin(); it != mutada.deltas.end(); ++it)
            if ((*it).first != xi)
                (*it).second -= p.get_d()[xi][(*it).first];
        mutada.deltas.erase(xi);

        // Ahora ya añadimos el xj
        mutada.deltas[xj] = mutada.delta(xj, p.get_d());
        // Y sumamos el delta que añade al resto
        for (auto it = mutada.deltas.begin(); it != mutada.deltas.end(); ++it)
            if ((*it).first != xj)
                (*it).second += p.get_d()[xj][(*it).first];
    }
    mutada.check_max_min();

    return mutada;
}

Solution::Solution() {
    min_delta = max_delta = diff = 0;
}

vector<int> range(int init, int fin) {
    assert(fin > init);
    vector<int> v(fin - init);
    for (int i = init; i < fin; i++)
        v[i - init] = i;
    return v;
}

Solution::Solution(int size) {
    selected.resize(size);
    for (int i = 0; i < selected.size(); i++)
        selected[i] = 0;
    max_delta = min_delta = 0;
}


// Generar los M padres aleatoriamente eligiendo n posiciones que poner a 1
vector<Solution> GeneraPoblacion(int M, Problem problema) {
    vector<Solution> Padres(M);
    vector<int> posiciones = range(0, problema.get_n());
    int pos;
    for (int i = 0; i < Padres.size(); i++) {
        vector<bool> selecteds(problema.get_n(), false);
        Random::shuffle(posiciones);
        pos = 0;
        for (int i = 0; i < problema.get_m(); i++) {
            selecteds[posiciones[i]] = true;
        }
        Padres[i] = Solution(selecteds, problema);
    }
    return Padres;
}

Solution Solution::cruce_multiple(const vector<Solution>& padres, const Problem& p) const {
    vector<bool> h(p.get_m(), false);
    vector<int> seleccionados = Random::get<vector>(0,static_cast<int>(padres.size()),p.get_m());
    for (int i=0; i<h.size(); i++) {
        if (seleccionados[i]==0)
            h[i] = selected[i];
        else
            h[i] = padres[seleccionados[i]-1].selected[i];
    }
    return (Solution(repare(h,p),p));
}


