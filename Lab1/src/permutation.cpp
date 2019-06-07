#include <permutation.hh>
#include <iostream>
#include <algorithm>
#include "cmax.hh"

using namespace std;

vector <vector<uint32_t>> Permutation::permute_list(vector<uint32_t> origin_list) {
    vector<vector<uint32_t>> list_of_permutions;

    do {
        list_of_permutions.push_back(origin_list);
    } while (next_permutation(origin_list.begin(), origin_list.end()));

    return list_of_permutions;
}

void Permutation::print_permuted_list(vector <vector<uint32_t>> permutation_list) {
    for(auto i : permutation_list) {
        for(auto j : i) cout << j << " ";
        cout << endl;
    }
}

void Permutation::print_permuted_list_with_cmax(vector <vector<uint32_t>> permutation_list, vector <vector<uint32_t>> vmachines) {
    cout << "+-----------------------------+" << endl;
    cout << "| Order       | Cmax" << endl;
    for(auto i : permutation_list) {
        cout << "+-----------------------------+" << endl;
        cout << "| ";
        for(auto j : i) cout << j+1 << " ";
        cout << "    |  " << Cmax::get_cmax(vmachines, i) << endl;
    }
    cout << "+-----------------------------+" << endl;
}
