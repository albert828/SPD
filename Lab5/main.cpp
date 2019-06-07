#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>

#define PRINT 0
#define MESSAGES 0
#define CTIME_SHRAGE 0

using namespace std;

struct Task
{
    int r;
    int p;
    int q;
    int cTime;
};

struct Info
{
    vector<Task> tasks;
    int cmax;
};
int rq0(const vector<Task> &tasks)
{
    int qsum = 0;
    int rsum = 0;
    int cmax = 0;

    for (const auto &i : tasks)
    {
        qsum += i.q;
        rsum += i.r;
        cmax += i.p;
    }
    return cmax;
}
void print_tasks(const vector<Task> &tasks)
{
    for (const auto &i : tasks)
    {
        cout << i.r << " " << i.p << " " << i.q << endl;
    }
    cout << endl;
}

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
        cout << "Error opennig file";
        exit(-1);
    }

    file >> _tasks;

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

int sum_p(const vector<Task> &tasks) noexcept
{
    int sum = 0;

    for (auto task : tasks)
    {
        sum += task.p;
    }

    return sum;
}

int sum_r(const vector<Task> &tasks) noexcept
{
    int sum = 0;

    for (auto task : tasks)
    {
        sum += task.r;
    }

    return sum;
}

int sum_q(const vector<Task> &tasks) noexcept
{
    int sum = 0;

    for (auto task : tasks)
    {
        sum += task.q;
    }

    return sum;
}

Task min_task_by_q(const vector<Task> &tasks) noexcept
{
    Task min;
    min.q = 100000;

    for (auto task : tasks)
    {
        if (task.q < min.q)
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


vector<Task> slice(const vector<Task>  &v, int start=0, int end=-1) {
    int oldlen = v.size();
    int newlen;

    if (end == -1 or end >= oldlen){
        newlen = oldlen-start;
    } else {
        newlen = end-start;
    }

    vector<Task> nv(newlen);

    for (int i=0; i<newlen; i++) {
        nv[i] = v[start+i];
    }
    return nv;
}

Info shrage(vector<Task> tasksN)
{
    Info info;
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
            t += e.p;
            cmax = max(cmax, t + e.q);
            //ktoras z tych opcji poprawna xD tak chyba nie uwzgledniamy luk ale liczy poprawnie c wiec zostaje
            //e.cTime = (cmax - e.q);
            e.cTime = t;
            pi.push_back(e);
        }
    }
    info.cmax = cmax;
    info.tasks = move(pi);
    return info;
}

int shrage_pmtn(vector<Task> tasks) {
    int cmax = 0;
    int t = 0;
    int q = 100000;
    Task l; l.r = 0; l.p = 0; l.q = 0;
    vector<Task> tasksG;
    vector<Task> tasksN = tasks;

    while(!tasksG.empty() ||
          !tasksN.empty()) {
        while(!tasksN.empty() &&
              min_task_by_r(tasksN).r <= t) {
            Task j = min_task_by_r(tasksN);
            tasksG.push_back(j);
            tasksN = remove_task(tasksN, j);
            if (j.q > l.q) {
                l.p = t - j.r;
                t = j.r;
                if (l.p > 0)
                    tasksG.push_back(l);
            }
        }

        if (tasksG.empty())
            t = min_task_by_r(tasksN).r;
        else {
            Task j = max_task_by_q(tasksG);
            tasksG = remove_task(tasksG, j);
            l = j;
            t += j.p;
            cmax = max(cmax, t + j.q);
        }
    }
    return cmax;
}

int calculate_b(const vector<Task> &tasks, const int cmax) noexcept
{
    int b{-1};
    int j{0};
    for(auto const task : tasks)
    {
        if(cmax == ( task.cTime + task.q ) )
            b = j;
        ++j;
    }
    return b;
}

int calculate_c(const vector<Task> &tasks, const int a, const int b) noexcept
{
    int c{-1};
    for(auto j{a}; j < b; j++)
    {
        if(tasks[j].q < tasks[b].q)
            c = j;
    }
    return c;
}

int calculate_a(const vector<Task> &tasks, const int cmax, const int b) noexcept
{
    int a{-1};
    for(auto j{0}; j < b; ++j)
    {
        int sum{0};
        for(auto s{j}; s < b; ++s)
            sum += tasks[s].p;
        if(cmax == ( tasks[j].r + sum + tasks[b].q ))
        {
            a = j;
            break;
        }
    }
    return a;
}

vector<Task> insert(vector<Task> tasks, Task task, int index) {
    tasks.insert(tasks.begin() + index, task);
    return tasks;
}

int cmax(vector<Task> tasks) {
    int M = 0;
    int cmax = 0;

    for (auto task : tasks) {
        M = max(M, task.r) + task.p;
        cmax = max(cmax, M + task.q);
    }
    return cmax;
}

int carlier(vector<Task> tasks)
{
    Info info;
    static int UB = INT_MAX;
    vector<Task> K, Kprim;
    info = shrage(tasks);
    int U = info.cmax;
    vector<Task> pi = info.tasks;
    int LB;
    int rK, pK, qK, rKp, pKp, qKp, rPiTmp, qPiTmp;
    if(U < UB)
    {
        UB = U;
    }
    int b = calculate_b(pi, U);
    int a = calculate_a(pi, U, b);
    int c = calculate_c(pi, a, b);
#if MESSAGES == 1
    cout << "From Carlier" << endl;
    cout << "UB: " << UB << " U: " << U << " a: " << a
         << " b: " << b << " c: " << c << endl;
#endif
    if (c < 0)
        return UB;

    K = slice(pi, c+1, b+1);
    rK = min_task_by_r(K).r;
    qK = min_task_by_q(K).q;
    pK = sum_p(K);
    int hK = rK + qK + pK;

    Kprim = insert(K, pi[c], 0);
    rKp = min_task_by_r(Kprim).r;
    qKp = min_task_by_q(Kprim).q;
    pKp = sum_p(Kprim);
    int hKprim = rKp + qKp + pKp;

    rPiTmp = pi[c].r;
    pi[c].r = max(pi[c].r, rK+pK);
    LB = shrage_pmtn(tasks);
    LB = max(hK, max(hKprim, LB));
    if (LB < UB)
    {
        UB = min(UB, carlier(pi));
        return UB;
    }
    pi[c].r = rPiTmp;


    qPiTmp = pi[c].q;
    pi[c].q = max(pi[c].q, qK+pK);
    LB = shrage_pmtn(tasks);
    LB = max(hK, max(hKprim, LB));
    if (LB < UB)
    {
        UB = min(UB, carlier(pi));
        return UB;
    }
    pi[c].q = qPiTmp;
    return UB;
}

int main()
{
    vector<Task> tasks = read_file();
    cout << rq0(tasks);
    //cout << carlier(tasks) << endl;
    return 0;
}
