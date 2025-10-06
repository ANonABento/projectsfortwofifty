#include "core.h"

core::core(int i) : id(i), dq(4) {}

int core::getId() { 
    return id; 
}

void core::addTask(int pid) {
    dq.pushBack(pid);
}

int core::runTask() {
    return dq.popFront();
}

int core::stealTask() {
    return dq.popBack();
}

int core::taskCount() {
    return dq.getSize();
}

int core::getCap() {
    return dq.getCap();
}

bool core::hasTasks() {
    return !dq.isEmpty();
}
