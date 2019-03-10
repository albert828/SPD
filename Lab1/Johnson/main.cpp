#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>

using namespace std;
/*
class Cmax {
public:
    Cmax() {};
    ~Cmax() {};

    static int get_cmax(sTask *tasks, int num_of_tasks) {
        int sum = tasks[0].time1;
        int tmp_span = 0;
        for(int i = 1; i < num_of_tasks; i++) {
            sum += tasks[i].time1;
            if(tmp_span == 0)
                tmp_span = tasks[i-1].time2;

            if(tmp_span <= tasks[i].time1) {
                if(num_of_tasks - 1 != i)
                    tmp_span = 0;
            } else {
                tmp_span = tmp_span + tasks[i].time2 - tasks[i].time1;
            }
        }
        sum += tmp_span;
        return sum;
    }
};
*/

/*
####Algorytm Johnsona####

S� 2 wersje tego algorytmu. Dzia�aj� tak samo ale r�ni� si� wykonaniem.
v1

Podzia� na 2 tablice. W pierwszej elementy o mniejszym czasie wykonania na M1 ni� na M2 posortowane rosn�co.
Natomiast w drugiej pozosta�e posortowane malej�co.
linki: http://kkapd.f11.com.pl/zsw/algorytm_johnsona.htm
        http://kkapd.f11.com.pl/zsw/Algorytm_Johnsona/przyklad_Johnson.htm

v2
Wstawianie w tablic� zada� wed�ug rosn�cego czasu od kraw�dzi zaczynaj�c z prawej strony.
linki: https://en.wikipedia.org/wiki/Johnson%27s_rule

Drugie chyba prostsze wi�c to zrobi�
*/


int32_t read_file(const string &filename, vector<vector<uint32_t>> &vmachines) {
    ifstream file;
    file.open(filename.c_str(), ios_base::in);
    if(!file.good())
        return -1;  // -1 to kod bledu
    //Wczytuje ilosc zadan
    /*
    // Format danych wejsciowych:
    // 3 2
    // 1 5
    // 2 5
    // 2 5
    */
    int32_t number_of_tasks{0};
    int32_t num_of_machines{0};
    file >> number_of_tasks >> num_of_machines;

    vmachines.resize(num_of_machines, vector<uint32_t>(number_of_tasks));
    for (uint16_t task{0}; task < number_of_tasks; ++task)
    {   //Wczytuje czasy wykonania
        uint32_t value{0};
        for(uint16_t list{0}; list < num_of_machines; ++list)
        {
            file >> value;
            vmachines[list][task] = value;
        }
    }
    return number_of_tasks;
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

int main()
{

    vector<vector<uint32_t>> vmachines, vmachines_copy;
    string filename;
    int32_t number_of_tasks{0};

    cout << "Podaj nazwe pliku: " << endl;
    getline(cin,filename);

    number_of_tasks = read_file(filename, vmachines);
    vmachines_copy = vmachines;
    if( number_of_tasks == -1)
        return -1;
    /*
    for(auto &i : vmachines)
    {
        for(auto &j : i)
            cout << j << ",";
        cout << endl;
    }
    */

    vector<uint32_t> sorted_nr_tasks(number_of_tasks);
    for(uint32_t i{0}; i < number_of_tasks; ++i)
    {
        uint32_t dist{0};
        dist = find_min_value(vmachines_copy);
        //cout << dist << " ";
        for(auto &list : vmachines_copy)
            list[dist] = UINT32_MAX;
/*
        for(const auto &list : vmachines)
        {
            for(const auto task : list)
                cout << task << ",";
            cout << endl;
        }
*/
        static bool is_end{true};
        if(is_end == true)
        {
            add_from_end(sorted_nr_tasks, dist, number_of_tasks);
            is_end = false;
        }
        else
        {
            add_from_begin(sorted_nr_tasks, dist, number_of_tasks);
            is_end = true;
        }
    }

for(auto i : sorted_nr_tasks)
    cout << (i + 1) << " " << endl;

//cout << "Sum:" << Cmax::get_cmax(tab_of_tasks, number_of_tasks) << endl;

    return 0;
}
