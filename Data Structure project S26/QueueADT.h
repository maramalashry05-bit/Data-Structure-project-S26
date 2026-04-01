#pragma once
#ifndef QUEUE_ADT_
#define QUEUE_ADT_

template<class T>
class QueueADT
{
public:
    //To check if the queue is empty.
     
    virtual bool isEmpty() const = 0;

    //To add a new entry to the back of this queue.
     
    virtual bool enqueue(const T& newEntry) = 0;

    // Copies the front item of the queue to the passed parameter and removes it from queue
  
    virtual bool dequeue(T& FrontEntry) = 0;

    // Copies the front item of the queue to the passed parameter without removing it from queue
     
    virtual bool peek(T& FrontEntry) const = 0;

    //Destroys this queue and frees its memory.
    virtual ~QueueADT() {}
};
#endif
