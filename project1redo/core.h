#ifndef core_h
#define core_h

#include "dq.h"

class core {
private:
    int id;
    dq dq;

public:
    core(int i);
    int getId();
    void addTask(int pid);
    int runTask();
    int stealTask();
    int taskCount();
    int getCap();
    bool hasTasks();
};

#endif
