#pragma once
#include "priNode.h"


//This class impelements the priority queue as a sorted list (Linked List)
//The item with highest priority is at the front of the queue
template <typename T>
class priQueue
{
protected:  
    priNode<T>* head;
public:
    priQueue() : head(nullptr) {}

    ~priQueue() {
        T *tmp;
        int p;
        while (dequeue(*tmp, p));
    }

    //insert the new node in its correct position according to its priority
    void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || priority > head->getPri()) {

            newNode->setNext(head);
            head = newNode;
            return;
        }

        priNode<T>* current = head;
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext(current->getNext());
        current->setNext(newNode);
    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        return true;
    }

    bool peek(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    // Copy Constructor
    priQueue(const priQueue<T>& other) {
        head = nullptr;

        priNode<T>* current = other.head;
        int pri = current->getPri();
        while (current) {
            enqueue(current->getItem(pri), current->getPri());
            current = current->getNext();
        }
    }

    void print() const {
        priNode<T>* current = head;

        cout << "Head -> ";
        while (current) {
            int pri;
            T val = current->getItem(pri);
            cout << "(" << val << ", p=" << pri << ") -> ";
            current = current->getNext();
        }
        cout << "NULL\n";
    }
};