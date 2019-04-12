#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <ctime>
#include <cmath>
#include <cstdlib>

#define TEMP_0 200
#define COEF_COLD 0.9
#define MIN_TEMP 5
#define MAX_ITERATE 100000
#define SWAP 0
#define START_ORDER_SORTED 1
#define NEH 1
#define PROBABILITY 1
#define DIFFERENT_CMAX 1

using namespace std;

int cost(const vector<int> tasks) {
    int sum = 0;
    for (auto i:tasks) sum += i;
    return sum;
}

bool compare(const vector<int> &tasks, const vector<int> &tasks2) {
    return cost(tasks)>cost(tasks2);
}

void sorted_neh(vector<vector<int>> &tasks) {
    sort(tasks.begin(), tasks.end(), compare);
}

int get_cmax(const vector<vector<int>> &vmachines) {
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

vector<vector<int>> slice(const vector<vector<int>>  &v, int start=0, int end=-1) {
    int oldlen = v.size();
    int newlen;

    if (end == -1 or end >= oldlen){
        newlen = oldlen-start;
    } else {
        newlen = end-start;
    }

    vector<vector<int>> nv(newlen);

    for (int i=0; i<newlen; i++) {
        nv[i] = v[start+i];
    }
    return nv;
}

void get_cmax_neh(vector<vector<int>> &tasks, int print) {
    sorted_neh(tasks);

    vector<vector<int>> base = slice(tasks,0,1);
    vector<vector<int>> tmp = slice(tasks,0,1);
    vector<vector<int>> tmp2 = slice(tasks,0,1);

    int glob_min=0;
    for(int i=1;i<tasks.size();i++) {
        int min=10000000;
        for(int j=0;j<i+1;j++) {
            tmp.insert(tmp.begin()+j,tasks[i]);
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
    tasks = base;
    if (print == 1)
        cout << "Cmax for NEH: " << glob_min << endl;
}

void print_tasks(const vector<vector<int>> &tasks) {
    for (auto i : tasks) {
        for (auto j:i) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;
}

vector<vector<int>> read_file() {
    vector<vector<int>> tasks;
    int _tasks = 0;
    int _machines = 0;
    string filename;

    cout << "Podaj nazwe pliku: ";
    cin >> filename;

    ifstream file;
    file.open(filename.c_str(), ios_base::in);
    if(!file.good()) {
        exit(-1);
    }

    file >> _tasks >> _machines;

    int val = 0;
    for (int i = 0; i < _tasks; ++i) {
        vector<int> vm;
        for(int j = 0; j < _machines; ++j) {
            file >> val;
            vm.push_back(val);
        }
        tasks.push_back(vm);
    }

    file.close();
    return tasks;
}

vector<vector<int>> swap(vector<vector<int>> tasks, int first, int second) {
    vector<int> tmp = tasks[first];
    tasks[first] = tasks[second];
    tasks[second] = tmp;
    return tasks;
}

vector<vector<int>> insert(vector<vector<int>> tasks, int source, int target) {
    vector<int> tmp = tasks[source];
    tasks.erase(tasks.begin() + source);
    tasks.insert(tasks.begin() + target, tmp);
    return tasks;
}

vector<vector<int>> init(vector<vector<int>> tasks, float &temp) {
    temp = TEMP_0;
    if (START_ORDER_SORTED == 1) get_cmax_neh(tasks, 0);
    return tasks;
}

vector<vector<int>> generate_neighbour(const vector<vector<int>> &tasks) {
    if (SWAP == 1)
        return swap(tasks, rand() % tasks.size(), rand() % tasks.size());
    return insert(tasks, rand() % tasks.size(), rand() % tasks.size());
}

float P(const vector<vector<int>> &inited_tasks, const vector<vector<int>> &neighbour_tasks, float temp) {
    int cmax = get_cmax(inited_tasks);
    int cmax_prim = get_cmax(neighbour_tasks);
    if(DIFFERENT_CMAX == 1)
    {
        if(cmax != cmax_prim)
        {
            if(PROBABILITY == 1)
                return exp((cmax-cmax_prim)/temp);
            else
                return cmax_prim > cmax ? exp((cmax-cmax_prim)/temp) : 1;
        }
        else
            return (-1.0);
    }
    else
    {
        if(PROBABILITY == 1)
            return exp((cmax-cmax_prim)/temp);
        else
            return cmax_prim > cmax ? exp((cmax-cmax_prim)/temp) : 1;
    }
}

vector<vector<int>> exec_move(const vector<vector<int>> &tasks, const vector<vector<int>> &neighbour_tasks, float temp) {
    float p = P(tasks, neighbour_tasks, temp);
    if(p == (-1.0) )
        return tasks;
    else
    {
        float rand_0_1 = (float) rand()/RAND_MAX;
        return p >= rand_0_1 ? neighbour_tasks : tasks;
    }
}

void cold(float &temp) {
    temp *= COEF_COLD;
}

void annealing(vector<vector<int>> tasks) {
    float temp = 0;
    vector<vector<int>> neighbour_tasks;

    tasks = init(tasks, temp);
    //print_tasks(tasks);
    for (int i = 0; i < MAX_ITERATE; i++) {
        neighbour_tasks = generate_neighbour(tasks);
        tasks = exec_move(tasks, neighbour_tasks, temp);
        cold(temp);
        if ((MIN_TEMP != 0) && (temp <= MIN_TEMP)) break;
    }

    // print_tasks(tasks);
    cout << "Cmax: " << get_cmax(tasks) << endl;
}

int main() {
    char a;
    srand (time (0));

    again:
    vector<vector<int>> tasks = read_file();

    annealing(tasks);
    if (NEH == 1) get_cmax_neh(tasks, 1);

    cout << "Powtorzyc? y/n";
    char decision{};
    cin >> decision;
    if(decision == 'y')
        goto again;

    return 0;
}
