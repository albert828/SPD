#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <ctime>
#include <cmath>
#include <cstdlib>

using namespace std;

struct Task {
  int r;
  int p;
  int q;
};

void print_tasks(vector<Task> tasks) {
    for (auto i : tasks) {
      cout << i.r << " " << i.p << " " << i.q << endl;
    }
    cout << endl;
}

vector<Task> read_file() {
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

    int val = 0;
    for (int i = 0; i < _tasks; ++i) {
      Task inst;
      file >> inst.r;
      file >> inst.p;
      file >> inst.q;
      tasks.push_back(inst);
    }

    file.close();
    return tasks;
}

Task min_task_by_r(vector<Task> tasks) {
  Task min;
  min.r = 100000;
  
  for (auto task : tasks) {
    if (task.r < min.r)
      min = task;
      
  }
  
  return min;
}

Task max_task_by_q(vector<Task> tasks) {
  Task max;
  max.q = 0;
  
  for (auto task : tasks) {
    if (task.q > max.q)
      max = task;
      
  }
  
  return max;
}


vector<Task> remove_task(vector<Task> tasks, Task removed_task) {
  vector<Task> new_tasks;
  for (auto task : tasks) {
    if (task.r == removed_task.r &&
	task.p == removed_task.p &&
	task.q == removed_task.q)
      continue;
    else
      new_tasks.push_back(task);
  }
  return new_tasks;
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

int main() {
    vector<Task> tasks = read_file();
    cout << shrage_pmtn(tasks) << endl;
    return 0;
}
