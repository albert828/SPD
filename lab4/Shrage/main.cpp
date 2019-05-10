#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define COMPILE_PRINT 0

using namespace std;

struct Task
{
    int r;
    int p;
    int q;
};
#if COMPILE_PRINT == 1
void print_tasks(const vector<Task> &tasks)
{
    for (const auto &i : tasks)
    {
        cout << i.r << " " << i.p << " " << i.q << endl;
    }
    cout << endl;
}
#endif
vector<Task> read_file()
{
    vector<Task> tasks;
    int _tasks = 0;
    int _count = 0;
    string filename;

    cout << "Podaj nazwe pliku: ";
    cin >> filename;

    ifstream file;
    file.open(filename.c_str(), ios_base::in);
    if(!file.good()) {
        exit(-1);
    }

    file >> _tasks >> _count;

    for (int i = 0; i < _tasks; ++i)
    {
        Task inst;
        file >> inst.r;
        file >> inst.p;
        file >> inst.q;
        tasks.push_back(inst);
    }

    file.close();
    return tasks;
}

Task min_task_by_r(const vector<Task> &tasks) noexcept
{
    Task min;
    min.r = 100000;

    for (auto task : tasks)
    {
        if (task.r < min.r)
            min = task;
    }

    return min;
}

Task max_task_by_q(const vector<Task> &tasks) noexcept
{
    Task max;
    max.q = 0;

    for (auto task : tasks)
    {
        if (task.q > max.q)
            max = task;
    }
    return max;
}


vector<Task> remove_task(const vector<Task> &tasks, const Task &removed_task)
{
    vector<Task> new_tasks;
    for (const auto &task : tasks)
    {
        if (task.r == removed_task.r &&
            task.p == removed_task.p &&
            task.q == removed_task.q)
            continue;
        else
            new_tasks.push_back(task);
    }
    return new_tasks;
}

int shrage(vector<Task> &tasksN)
{
    int cmax = 0;
    int t = 0;
    int k = 0;

    vector<Task> tasksG;
    vector<Task> pi;

    while(!tasksG.empty() || !tasksN.empty())
    {
        while(!tasksN.empty() && min_task_by_r(tasksN).r <= t)
        {
            Task e = min_task_by_r(tasksN);
            tasksG.push_back(e);
            tasksN = remove_task(tasksN, e);
        }

        if (tasksG.empty())
            t = min_task_by_r(tasksN).r;
        else
        {
            Task e = max_task_by_q(tasksG);
            tasksG = remove_task(tasksG, e);
            k += 1;
            pi.push_back(e);
            t += e.p;
            cmax = max(cmax, t + e.q);
        }
    }
    return cmax;
}

int main()
{
    vector<Task> tasks = read_file();
    cout << shrage(tasks) << endl;
    return 0;
}