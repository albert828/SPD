#include <iostream>
#include <vector>
#include <algorithm>
/*
    Example of usage:
    vector <int> list;
    vector <vector<int>> list1;
    list.push_back(1);
    list.push_back(3);
    list.push_back(5);
    list1 = Permutation::permute_list(list);
    Permutation::print_permuted_list(list1);
*/
class Permutation {
    public:
    static vector<vector<int>> permute_list(vector<int> origin_list, int low = 0) {
        vector<vector<int>> list_of_permutions;

        do {
            list_of_permutions.push_back(origin_list);
        } while (next_permutation(origin_list.begin(), origin_list.end()));

        return list_of_permutions;
    };

    static void print_permuted_list(vector <vector<int>> list) {
        for(auto i : list) {
            for(auto j : i) cout << j << " ";
            cout << endl;
        }
    };
};