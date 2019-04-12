#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

int main()
{
    int number_of_tasks = 0;
    int number_of_machines = 0;
    int range_of_value = 0;
    cout << "Privide amount of tasks: ";
    cin >> number_of_tasks;
    cout << "Privide amount of machines: ";
    cin >> number_of_machines;
    cout << "Privide range of random numbers: ";
    cin >> range_of_value;
    int max_len_of_range = to_string(range_of_value).length();

    ofstream file;
    file.open ("data.txt");
    file << setw(max_len_of_range) << number_of_tasks << setw(max_len_of_range+1) << number_of_machines << endl;
    int rand_num = 0;
    for(int i = 0; i < number_of_tasks; i++) {
        for(int j = 0; j < number_of_machines; j++) {
            rand_num = rand() % range_of_value + 1;
            file << setw(max_len_of_range) << rand_num << " "; 
        }
        file << endl;
    }
    file.close();

    return 0;
}
