#include "dq.h"

dq::dq(int startCap) {
    if (startCap < 2) {
        startCap = 2;
    }
    cap = startCap;
    arr = new int[cap];
    size = 0;
    front = 0;
}

dq::~dq() {
    delete[] arr;
}

void dq::resize(int newCap) {
    if (newCap < 2) {
        newCap = 2;
    }
    int *temp = new int[newCap];
    for (int i = 0; i < size; i++) {
        temp[i] = arr[(front + i) % cap];
    }
    delete[] arr;
    arr = temp;
    cap = newCap;
    front = 0;
}

void dq::pushBack(int x) {
    if (size == cap) {
        resize(cap * 2);
    }
    int idx = (front + size) % cap;
    arr[idx] = x;
    size++;
}

void dq::pushFront(int x) {
    if (size == cap) {
        resize(cap * 2);
    }
    front = (front - 1 + cap) % cap;
    arr[front] = x;
    size++;
}

int dq::popBack() {
    if (size == 0) {
        return -1;
    }
    int idx = (front + size - 1) % cap;
    int val = arr[idx];
    size--;
    if (size <= cap / 4 && cap > 2) {
        resize(cap / 2);
    }
    return val;
}

int dq::popFront() {
    if (size == 0) {
        return -1;
    }
    int val = arr[front];
    front = (front + 1) % cap;
    size--;
    if (size <= cap / 4 && cap > 2) {
        resize(cap / 2);
    }
    return val;
}

int dq::getSize() { 
    return size; 
}
int dq::getCap() { 
    return cap; 
}
bool dq::isEmpty() {
    if (size == 0) {
        return true;
    }
    else {
        return false;
    }
}