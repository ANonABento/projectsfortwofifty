#ifndef cpu_h
#define cpu_h

#include "core.h"

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
