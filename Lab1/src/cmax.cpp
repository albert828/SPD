#include "cmax.hh"

using namespace std;

int Cmax::get_cmax(vector<vector<uint32_t>> vmachines, vector<uint32_t> sorted) {
    vector<int> sum;
    for(vector<int>::size_type i = 0; i != vmachines.size(); i++) {
        sum.push_back(0);
    }
    for(vector<int>::size_type i = 0; i != sorted.size(); i++) {
        sum[0] += vmachines[0][sorted[i]];
        for(vector<int>::size_type j = 1; j != vmachines.size(); j++) {
            if(sum[j] < sum[j-1]) sum[j] = sum[j-1];
            sum[j] += vmachines[j][sorted[i]];
        }
    }
    int max = 0;
    for(vector<int>::size_type i = 0; i != vmachines.size(); i++) {
        if(sum[i] > max) max=sum[i];
    }
    // return ma#include "cmax.hh"x;
    return sum.back();
}
