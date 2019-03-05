#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct sTask
{
    uint32_t nr_of_task;
    uint32_t time;
};

/*
####Algorytm Johnsona####

S¹ 2 wersje tego algorytmu. Dzia³aj¹ tak samo ale ró¿ni¹ siê wykonaniem.
v1

Podzia³ na 2 tablice. W pierwszej elementy o mniejszym czasie wykonania na M1 ni¿ na M2 posortowane rosn¹co.
Natomiast w drugiej pozosta³e posortowane malej¹co.
linki: http://kkapd.f11.com.pl/zsw/algorytm_johnsona.htm
        http://kkapd.f11.com.pl/zsw/Algorytm_Johnsona/przyklad_Johnson.htm

v2
Wstawianie w tablicê zadañ wed³ug rosn¹cego czasu od krawêdzi zaczynaj¹c z prawej strony.
linki: https://en.wikipedia.org/wiki/Johnson%27s_rule

Drugie chyba prostsze wiêc to zrobiê
*/

int32_t read_file(const string &filename, vector<uint32_t> &vmachine1, vector<uint32_t> &vmachine2)
{
    ifstream file;
    file.open(filename.c_str(), ios_base::in);
    if(!file.good())
        return -1;

    int32_t number_of_machines{0};
    file >> number_of_machines;

    for (uint16_t counter{0}; counter < number_of_machines; ++counter)
    {
        uint32_t time1, time2;
        file >> time1 >> time2;
        vmachine1.push_back(time1);
        vmachine2.push_back(time2);
    }


    return number_of_machines;
}

void add_from_end(uint32_t *tab_of_tasks, uint32_t value, int32_t number_of_machines)
{
    static int32_t position{number_of_machines -1};
    if(position >= 0)
    {
        tab_of_tasks[position] = value;
        --position;
    }
    else
        cout << "Wyjechalismy poza dolny zakres tablicy :(" << endl;
}

void add_from_begin(uint32_t *tab_of_tasks, uint32_t value, int32_t number_of_machines)
{
    static uint32_t position{0};
    if(position <= number_of_machines)
    {
        tab_of_tasks[position] = value;
        ++position;
    }
    else
        cout << "Wyjechalismy poza gorny zakres tablicy :(" << endl;
}


int main()
{
    vector<uint32_t> vmachine1, vmachine2, both_machines;
    string filename;
    int32_t number_of_machines{0};

    cout << "Podaj nazwe pliku: " << endl;
    getline(cin,filename);

    number_of_machines = read_file(filename, vmachine1, vmachine2);
    if( number_of_machines == -1)
        return -1;
    both_machines = vmachine1;
    both_machines.insert( both_machines.end(), vmachine2.begin(), vmachine2.end() );
    //for(auto &i : both_machines)
       // cout << i << endl;


    uint32_t *tab_of_tasks = new uint32_t [number_of_machines];

    for(uint32_t counter{0}; counter < number_of_machines; ++counter)
    {
        auto result = min_element(begin(both_machines), end(both_machines));
        //cout << *result << endl;

        static bool is_end{true};
        if(is_end == true)
        {
            add_from_end(tab_of_tasks, *result, number_of_machines);
            both_machines.erase(result);
            is_end = false;
        }
        else
        {
            add_from_begin(tab_of_tasks, *result, number_of_machines);
            both_machines.erase(result);
            is_end = true;
        }

    }

    for(uint32_t element{0}; element < number_of_machines; ++element)
        cout << tab_of_tasks[element] << endl;

    delete []tab_of_tasks;
    tab_of_tasks = nullptr;

    return 0;
}
