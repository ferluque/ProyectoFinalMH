//
// Created by Fernando on 07/05/2022.
//

#include <iostream>
#include "random.hpp"
#include "Solution.h"
using namespace std;
using Random = effolkronium::random_static;


int main() {

    vector<int> r = Random::get<vector>(0,10,5);
    cout << r << endl;

}