#ifndef dq_h
#define dq_h

class dq {
private:
    int *arr;
    int cap;
    int size;
    int front;

    void resize(int newCap);

public:
    dq(int startCap = 4);
    ~dq();

    void pushBack(int x);
    void pushFront(int x);
    int popBack();
    int popFront();

    int getSize();
    int getCap();
    bool isEmpty();
};

#endif
