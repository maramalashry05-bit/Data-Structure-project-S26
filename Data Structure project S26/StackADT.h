#ifndef STACK_ADT_
#define STACK_ADT_

template<typename T>
class StackADT
{
public:
     //To check if the stack is empty
    virtual bool isEmpty() const = 0;
     //To add an item on the top of the stack
    virtual bool push(const T& newEntry) = 0;
    //To copy the top element to TopEntry and removes it from the stack 
    virtual bool pop(T& TopEntry) = 0;
    //To copy the top element to TopEntry without removing it from the stack 
    virtual bool peek(T& TopEntry) const = 0;
    //To destroy the stack and free its memory
       virtual ~StackADT() {}
}; 
#endif
