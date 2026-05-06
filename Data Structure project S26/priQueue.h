#pragma once
#include "priNode.h"
#include <iostream>
#include"Table.h"
#include <type_traits>
using namespace std;


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
        T *tmp=nullptr;
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

    // Return the number of elements in the priority queue
    int getCount() const {
        int count = 0;
        priNode<T>* current = head;
        while (current) {
            ++count;
            current = current->getNext();
        }
        return count;
    }

private:
    // Helper overloads to print ID for pointer and non-pointer item types
    template<typename U = T>
    static typename std::enable_if<std::is_pointer<U>::value, void>::type
        PrintIDHelper(const U& item)
    {
        if (item)
            cout << item->GetID() << " ";
        else
            cout << "(null) ";
    }

    template<typename U = T>
    static typename std::enable_if<!std::is_pointer<U>::value, void>::type
        PrintIDHelper(const U& item)
    {
        cout << item.GetID() << " ";
    }

public:
    // Print only the IDs of items (assumes items expose GetID())
    void printIDs() const {
        priNode<T>* current = head;
        while (current) {
            int p;
            T val = current->getItem(p);
            PrintIDHelper(val);
            current = current->getNext();
        }
        cout << endl;
    }
    void printIDsWithCompanions() const {                   // it print the Order Id and the resource ID 
        priNode<T>* current = head;
        while (current!=nullptr) {
            int p;
            T ord = current->getItem(p);
            if (ord) {
                cout << "[" << ord->GetID() << ", ";
                // Logic to find if it has a Scooter or Table assigned
                if (ord->getScooter()) cout << "S" << ord->getScooter()->GetID();
                else if (ord->getTable()) cout << "T" << ord->getTable()->getID();
                // If it's a cooking order, you might need to link to Chef ID 
                // but usually, Order ID alone is the priority here.
                cout << "]";
            }
            if (current->getNext()) cout << ", ";
            current = current->getNext();
        }
        cout << endl;
    }
};