#include "core.h"

core::core(int i) : id(i), dq1(4) {}

int core::getId() { 
    return id; 
}

void core::addTask(int pid) {
    dq1.pushBack(pid);
}

int core::runTask() {
    return dq1.popFront();
}

int core::stealTask() {
    return dq1.popBack();
}

int core::taskCount() {
    return dq1.getSize();
}

int core::getCap() {
    return dq1.getCap();
}

bool core::hasTasks() {
    return !dq1.isEmpty();
}
