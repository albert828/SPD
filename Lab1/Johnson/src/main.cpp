#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include "permutation.hh"
#include "cmax.hh"

using namespace std;


struct sTask_Info
{
    uint32_t number_of_machines;
    int32_t number_of_tasks;
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
    //Wczytuje ilosc zadan
    /*
    // Format danych wejsciowych:
    // 3 2
    // 1 5
    // 2 5
    // 2 5
    */
    //sTask_Info info;
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

uint32_t find_min_value(vector<vector<uint32_t>> &vmachines)
{
    vector<uint32_t>::iterator min1, min2;
    uint32_t dist;
    min1 = min_element(begin(vmachines[0]), end(vmachines[0]));
    min2 = min_element(begin(vmachines[1]), end(vmachines[1]));
    //cout << *min1 << " " << *min2 << endl;
    (*min1 < *min2) ? (dist = distance(begin(vmachines[0]), min1)) : dist = distance(begin(vmachines[1]),min2);
    //cout << dist << endl;
    return dist;
}

//Funkcja dodajaca od konca tablicy
void add_from_end(vector<uint32_t> &sorted_nr_tasks, const uint32_t value, const int32_t number_of_tasks)
{
    static int32_t position{number_of_tasks -1};
    if(position >= 0)
    {
        sorted_nr_tasks[position] = value;
        --position;
    }
    else
        cout << "Wyjechalismy poza dolny zakres tablicy :(" << endl;
}
//Funkcja dodajaca od poczatku tablicy
void add_from_begin(vector<uint32_t> &sorted_nr_tasks, const uint32_t value, const int32_t number_of_tasks)
{
    static uint32_t position{0};
    if(position <= number_of_tasks)
    {
        sorted_nr_tasks[position] = value;
        ++position;
    }
    else
        cout << "Wyjechalismy poza gorny zakres tablicy :(" << endl;
}
void connect_vectors(const vector<vector<uint32_t>> &vmachines, vector<vector<uint32_t>> &vmachines_connected, const sTask_Info &info)
{
    if(info.number_of_machines == 2)
        vmachines_connected = vmachines;
    else if(info.number_of_machines == 3)
    {
        for(uint32_t task{0}; task < info.number_of_tasks; ++task)
        {
            vmachines_connected[0][task] = (vmachines[0][task] + vmachines[1][task]);
            vmachines_connected[1][task] = (vmachines[1][task] + vmachines[2][task]);
        }
    }
    else
        cout << "Niepoprawna ilosc maszyn" << endl;
}

int main()
{

    vector<vector<uint32_t>> vmachines, vmachines_copy, vmachines_connected;
    string filename;
    sTask_Info info;

    cout << "Podaj nazwe pliku: " << endl;
    getline(cin,filename);

    info = read_file(filename, vmachines);
    if( info.number_of_tasks == -1)
        return -1;
    vmachines_copy = vmachines;
    vmachines_connected.resize(2,vector<uint32_t>(info.number_of_tasks));
    connect_vectors(vmachines, vmachines_connected, info);
    vmachines_copy = vmachines_connected;

    vector<uint32_t> sorted_nr_tasks(info.number_of_tasks);
    for(uint32_t i{0}; i < info.number_of_tasks; ++i)
    {
        uint32_t dist{0};
        dist = find_min_value(vmachines_copy);
        //cout << dist << " ";
        for(auto &list : vmachines_copy)
            list[dist] = UINT32_MAX;

        static bool is_end{true};
        if(is_end == true)
        {
            add_from_end(sorted_nr_tasks, dist, info.number_of_tasks);
            is_end = false;
        }
        else
        {
            add_from_begin(sorted_nr_tasks, dist, info.number_of_tasks);
            is_end = true;
        }
    }

    vector<uint32_t> tasks;
    for(int i = 0; i < info.number_of_tasks;i++) tasks.push_back(i);
    vector <vector<uint32_t>> all_permutation = Permutation::permute_list(tasks);
    Permutation::print_permuted_list_with_cmax(all_permutation, vmachines);

    cout << "+-----------------------------+" << endl;
    cout << "| Johnson Order  | Cmax" << endl;
    cout << "+-----------------------------+" << endl;
    cout << "| ";
    for(auto i : sorted_nr_tasks) cout << (i + 1) << " ";
    cout << "       |  " << Cmax::get_cmax(vmachines, sorted_nr_tasks);
    cout << endl;

    return 0;
}
