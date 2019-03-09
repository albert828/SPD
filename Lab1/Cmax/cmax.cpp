

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