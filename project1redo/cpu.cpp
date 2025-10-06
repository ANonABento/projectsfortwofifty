#include "cpu.h"
#include <iostream>

using namespace std;

cpu::cpu() {
    cores = nullptr;
    num = 0;
    active = false;
}

cpu::~cpu() {
    off();
}

bool cpu::on(int n) {
    if (active) return false;
    if (n <= 1) return false;
    num = n;
    cores = new core*[num];
    for (int i = 0; i < num; i++) {
        cores[i] = new core(i);
    }
    active = true;
    return true;
}

void cpu::off() {
    if (!active) return;
    for (int i = 0; i < num; i++) {
        delete cores[i];
    }
    delete[] cores;
    cores = nullptr;
    num = 0;
    active = false;
}

int cpu::findLeast() {
    int idx = 0;
    int smallest = cores[0]->taskCount();
    for (int i = 1; i < num; i++) {
        int s = cores[i]->taskCount();
        if (s < smallest) {
            smallest = s;
            idx = i;
        }
    }
    return idx;
}

int cpu::findMost(int skip) {
    int idx = -1;
    int biggest = -1;
    for (int i = 0; i < num; i++) {
        if (i == skip) continue;
        int s = cores[i]->taskCount();
        if (s > biggest || (s == biggest && idx > i)) {
            biggest = s;
            idx = i;
        }
    }
    return idx;
}

bool cpu::spawn(int pid, int &cid) {
    if (!active || pid <= 0) return false;
    cid = findLeast();
    cores[cid]->addTask(pid);
    return true;
}

bool cpu::run(int cid, int &pid, bool &empty) {
    if (!active || cid < 0 || cid >= num) return false;

    if (!cores[cid]->hasTasks()) {
        empty = true;
        int stealFrom = findMost(cid);
        if (stealFrom != -1 && cores[stealFrom]->hasTasks()) {
            int stolen = cores[stealFrom]->stealTask();
            if (stolen != -1) cores[cid]->addTask(stolen);
        }
        return true;
    }

    pid = cores[cid]->runTask();
    empty = false;

    if (!cores[cid]->hasTasks()) {
        int stealFrom = findMost(cid);
        if (stealFrom != -1 && cores[stealFrom]->hasTasks()) {
            int stolen = cores[stealFrom]->stealTask();
            if (stolen != -1) cores[cid]->addTask(stolen);
        }
    }

    return true;
}

bool cpu::sleep(int cid) {
    if (!active || cid < 0 || cid >= num) return false;

    if (!cores[cid]->hasTasks()) {
        std::cout << "core " << cid << " has no tasks to assign" << std::endl;
        return true;
    }

    while (cores[cid]->hasTasks()) {
        int pid = cores[cid]->stealTask();
        int best = -1;
        int bestSize = 999999;
        for (int i = 0; i < num; i++) {
            if (i == cid) continue;
            int s = cores[i]->taskCount();
            if (s < bestSize || (s == bestSize && i < best)) {
                best = i;
                bestSize = s;
            }
        }
        if (best == -1) break;
        cores[best]->addTask(pid);
        cout << "task " << pid << " assigned to core " << best << " ";
    }
    cout << std::endl;
    return true;
}

bool cpu::shutdown() {
    if (!active) return false;
    bool any = false;
    for (int i = 0; i < num; i++) {
        while (cores[i]->hasTasks()) {
            int pid = cores[i]->stealTask();
            std::cout << "deleting task " << pid << " from core " << i << " ";
            any = true;
        }
    }
    if (!any) std::cout << "no tasks to remove" << std::endl;
    return true;
}

bool cpu::size(int cid, int &out) {
    if (!active || cid < 0 || cid >= num) return false;
    out = cores[cid]->taskCount();
    return true;
}

bool cpu::capacity(int cid, int &out) {
    if (!active || cid < 0 || cid >= num) return false;
    out = cores[cid]->getCap();
    return true;
}

bool cpu::isOn() { return active; }
