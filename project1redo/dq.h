#ifndef dq_h
#define dq_h

/*
CITATION:
Parts of this class were discussed with chat.openai.com to clarify
how to correctly implement a circular dynamic array and handle resizing
logic. I wrote and debugged the final version myself to fit project
requirements and ensure proper memory management.
*/
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
