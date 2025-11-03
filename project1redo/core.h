#ifndef core_h
#define core_h

#include "dq.h"

/*
CITATION:
I reviewed ChatGPT's reviews of my inital class implementations
to fix syntax issues and ensure proper wrapping of the dq class.
Then I refined the design of this class myself to ensure it met
project requirements.
*/
class core {
private:
    int id;
    dq dq1;

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
