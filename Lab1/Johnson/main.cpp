#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct sTask
{
    uint32_t nr_of_task;
    uint32_t time1;
    uint32_t time2;
    uint32_t min_time;
};

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

int32_t read_file(const string &filename, vector<sTask> &vboth_machines)
{
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

    for (uint16_t counter{0}; counter < number_of_tasks; ++counter)
    {   //Wczytuje czasy wykonania
        sTask machine1;
        file >> machine1.time1 >> machine1.time2;
        //Wczytuje nr zadania +1 bo od 0 liczymy
        machine1.nr_of_task = counter + 1;
        vboth_machines.push_back(machine1);
    }
    return number_of_tasks;
}
//Funkcja dodajaca od konca tablicy
void add_from_end(sTask *tab_of_tasks, sTask value, const int32_t number_of_tasks)
{
    static int32_t position{number_of_tasks -1};
    if(position >= 0)
    {
        tab_of_tasks[position] = value;
        --position;
    }
    else
        cout << "Wyjechalismy poza dolny zakres tablicy :(" << endl;
}
//Funkcja dodajaca od poczatku tablicy
void add_from_begin(sTask *tab_of_tasks, sTask value, const int32_t number_of_tasks)
{
    static uint32_t position{0};
    if(position <= number_of_tasks)
    {
        tab_of_tasks[position] = value;
        ++position;
    }
    else
        cout << "Wyjechalismy poza gorny zakres tablicy :(" << endl;
}
//Funkcja pomocnicza do wyszukiwania minimalnego czasu
bool comparator(const sTask& s1, const sTask& s2)
{
    return s1.min_time < s2.min_time;
}

int main()
{

    vector<sTask>both_machines;
    string filename;
    int32_t number_of_tasks{0};

    cout << "Podaj nazwe pliku: " << endl;
    getline(cin,filename);

    number_of_tasks = read_file(filename, both_machines);
    if( number_of_tasks == -1)
        return -1;
    //wpisuje do zmiennej pomocniczej mniejszy czas zadania z obu maszyn
    for(auto &i : both_machines)
        (i.time1 < i.time2) ? (i.min_time = i.time1) : (i.min_time = i.time2);
    //Koncowa tablica wedlug prawidlowej kolejnosci
    sTask *tab_of_tasks = new sTask [number_of_tasks];

    for(uint32_t counter{0}; counter < number_of_tasks; ++counter)
    {   //Najmniejszy element z vectora
        auto result = min_element(begin(both_machines), end(both_machines), comparator);
        //Zmienna przelaczajaca miedzy wpisywaniem od konca a poczatkiem
        static bool is_end{true};
        if(is_end == true)
        {
            add_from_end(tab_of_tasks, *result, number_of_tasks);
            cout << "usuwam" << result->min_time << endl;
            both_machines.erase(result);
            is_end = false;
        }
        else
        {
            add_from_begin(tab_of_tasks, *result, number_of_tasks);
            cout << "usuwam" << result->min_time << endl;
            both_machines.erase(result);
            is_end = true;
        }
    }

    for(uint32_t element{0}; element < number_of_tasks; ++element)
        cout << tab_of_tasks[element].nr_of_task << "   " << tab_of_tasks[element].min_time << endl;

    cout << "Sum:" << Cmax::get_cmax(tab_of_tasks, number_of_tasks) << endl;
    delete []tab_of_tasks;
    tab_of_tasks = nullptr;

    return 0;
}
