//
// Created by Fernando on 07/05/2022.
//

#include "Solution.h"
#include "Problem.h"
#include <iostream>
#include <vector>
#include "random.hpp"

using namespace std;
using Random = effolkronium::random_static;


int main() {
    Random::seed(0);

    Problem p("..\\datos\\GKD-b_6_n25_m7.txt");

    Solution s = SolucionAleatoria(p);

    for (int i=0; i<s.get_selected().size();i++)
        p.extract(s.get_selected()[i]);

    Solution m = s.mutacion(p);

    cout << s;

    cout << m;
    m.print_dist("..\\resultados\\mutada.csv",p.get_d());

}