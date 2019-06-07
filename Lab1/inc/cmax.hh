#include <vector>
#include <stdint.h>

using namespace std;

class Cmax {
    public:
    Cmax() {};
    ~Cmax() {};

    static int get_cmax(vector<vector<uint32_t>> vmachines, vector<uint32_t> sorted);
};