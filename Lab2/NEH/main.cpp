#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>

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
    for (auto i : sorted_tasks)
        cout << i << ",";

    return 0;
}