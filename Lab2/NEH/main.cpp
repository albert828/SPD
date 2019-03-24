#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <ctime>

using namespace std;

struct sTask_Info
{
    uint32_t number_of_machines;
    int32_t number_of_tasks;
};
struct sTask
{
    uint32_t time;
    uint32_t index;

    bool operator < (const sTask& task) const
    {
        return (time > task.time); //odwrocenie sortowania
    }
};

int get_cmax(vector<vector<uint32_t>> vmachines, vector<uint32_t> sorted) {
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
    return sum.back();
}

int get_cmax(vector<vector<uint32_t>> vmachines) {
    vector<int> sum;
    for(vector<int>::size_type i = 0; i != vmachines[0].size(); i++) {
        sum.push_back(0);
    }
    for(vector<int>::size_type i = 0; i != vmachines.size(); i++) {
        sum[0] += vmachines[i][0];
        for(vector<int>::size_type j = 1; j != vmachines[i].size(); j++) {
            if(sum[j] < sum[j-1]) sum[j] = sum[j-1];
            sum[j] += vmachines[i][j];
        }
    }
    return sum.back();
}

vector<vector<uint32_t>> slice(vector<vector<uint32_t>>  v, int start=0, int end=-1) {
    int oldlen = v.size();
    int newlen;

    if (end == -1 or end >= oldlen){
        newlen = oldlen-start;
    } else {
        newlen = end-start;
    }

    vector<vector<uint32_t>> nv(newlen);

    for (int i=0; i<newlen; i++) {
        nv[i] = v[start+i];
    }
    return nv;
}

/*
    Create vector of vector with sorted tasks 
*/
vector<vector<uint32_t>> reform_vm(vector<vector<uint32_t>> vmachines, vector<uint32_t> sorted) {
    vector<vector<uint32_t>> vm;
    for(auto i : sorted) {
        vector<uint32_t> tmp;
        for(vector<int>::size_type j = 0; j != vmachines.size(); j++)
            tmp.push_back(vmachines[j][i]);
        vm.push_back(tmp);
    }
    return vm;
}

int get_cmax_neh(const vector<vector<uint32_t>> vmachines, vector<uint32_t> sorted) {
  
  vector<vector<uint32_t>> vm = reform_vm(vmachines, sorted);
  
  vector<vector<uint32_t>> base = slice(vm,0,1);
  vector<vector<uint32_t>> tmp = slice(vm,0,1);
  vector<vector<uint32_t>> tmp2 = slice(vm,0,1);
  int glob_min=0;
  for(int i=1;i<vm.size();i++) {
    int min=10000000;
    for(int j=0;j<i+1;j++) {
      tmp.insert(tmp.begin()+j,vm[i]);
      int v = get_cmax(tmp);
      if(min>v) {
        glob_min=v;
        min = v;
        base = tmp;
      }
      tmp = tmp2;
    }
    tmp2 = base;
    tmp = base;
  }
  return glob_min;
}


sTask_Info read_file(const string &filename, vector<vector<uint32_t>> &vmachines) {
    sTask_Info info;
    ifstream file;
    file.open(filename.c_str(), ios_base::in);
    if(!file.good())
    {
        info.number_of_tasks = -1;
        return info;  // -1 to kod bledu
    }

    file >> info.number_of_tasks >> info.number_of_machines;

    vmachines.resize(info.number_of_machines, vector<uint32_t>(info.number_of_tasks));
    for (uint16_t task{0}; task < info.number_of_tasks; ++task)
    {   //Wczytuje czasy wykonania
        uint32_t value{0};
        for(uint16_t list{0}; list < info.number_of_machines; ++list)
        {
            file >> value;
            vmachines[list][task] = value;
        }
    }
    return info;
}

vector<uint32_t> sort_tasks(const vector<vector<uint32_t>> &vmachines, const sTask_Info &info)
{
    vector<sTask> task_time(info.number_of_tasks);
    vector<uint32_t> task_index;
    task_index.reserve(info.number_of_tasks);

    for(uint32_t task{0} ; task < info.number_of_tasks; ++task)
    {
        uint32_t time_sum{0};
        for(uint32_t machine{0}; machine < info.number_of_machines ; ++machine)
        {
            time_sum += vmachines[machine][task];
        }
        task_time[task].time = time_sum;
        task_time[task].index = task;
    }
    //for (auto i : task_time)
        //cout << i.index << ",";
    stable_sort(begin(task_time), end(task_time));
    //for (auto i : task_time)
        //cout << i.index << ",";
    for (const auto &i : task_time)
        task_index.push_back(i.index);
    //for (auto i : task_index)
        //cout << i << ",";

    return task_index;
}


int main() {
    vector<vector<uint32_t>> vmachines;
    string filename;
    sTask_Info info;

    cout << "Podaj nazwe pliku: " << endl;
    getline(cin,filename);

    info = read_file(filename, vmachines);
    if( info.number_of_tasks == -1)
        return -1;
    vector<uint32_t> sorted_tasks(info.number_of_tasks);
    sorted_tasks = sort_tasks(vmachines, info);

    double duration;
    clock_t start = clock();
    cout << "Cmax for NEH: " << get_cmax_neh(vmachines, sorted_tasks) << endl;;
    duration = (clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "Time: " << duration << endl;
    return 0;
}
