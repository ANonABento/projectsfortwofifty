#ifndef cpu_h
#define cpu_h

#include "core.h"

/*
CITATION:
I consulted GPT-5 for general guidance on organizing
the CPU–core interaction and load-balancing logic. The specific
functions and control flow were written and adjusted by
me to match the project specifications.
I needed additional help with the sleep and run functions,
which I also discussed with GPT-5. I implemented it's suggestions
regarding loop logic and formatting issues.
*/
class cpu {
private:
    core **cores;
    int num;
    bool active;

    int findLeast();
    int findMost(int skip);

public:
    cpu();
    ~cpu();

    bool on(int n);
    void off();

    bool spawn(int pid, int &cid);
    bool run(int cid, int &pid, bool &empty);
    bool sleep(int cid);
    bool shutdown();

    bool size(int cid, int &out);
    bool capacity(int cid, int &out);
    bool isOn();
};

#endif
