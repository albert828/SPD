#include <vector>
#include <stdint.h>

using namespace std;

class Permutation {
    public:
    static vector<vector<uint32_t>> permute_list(vector<uint32_t> origin_list);
    static void print_permuted_list(vector <vector<uint32_t>> list);
    static void print_permuted_list_with_cmax(vector <vector<uint32_t>> list, vector <vector<uint32_t>> vmachines);
};