//
// Created by Fernando on 07/05/2022.
//

#include <iostream>
#include "random.hpp"
#include "Solution.h"
#include "bl.h"
using namespace std;
using Random = effolkronium::random_static;


int main() {
//    Random::seed(0);
    Problem p("..\\datos\\GKD-b_6_n25_m7.txt");
    Solution s = GeneraPoblacion(1, p)[0];
    cout << s << endl;

    s = bl(p, s).first;
    cout << s << endl;
}