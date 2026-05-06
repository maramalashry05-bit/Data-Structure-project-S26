#pragma once
#ifndef LINKED_QUEUE_
#define LINKED_QUEUE_

#include <iostream>
using namespace std;
#include "Node.h"
#include "QueueADT.h"

template <typename T>
class LinkedQueue : public QueueADT<T>
{
protected:  //in case you need to inherit for the project
    Node<T>* backPtr;
    Node<T>* frontPtr;
public:

    LinkedQueue()
    {
        backPtr = nullptr;
        frontPtr = nullptr;
    }

    bool isEmpty() const
    {
        return (frontPtr == nullptr);
    }

    bool enqueue(const T& newEntry)
    {
        Node<T>* newNodePtr = new Node<T>(newEntry);
        // Insert the new node
        if (isEmpty()) // special case if this is the first node to insert
            frontPtr = newNodePtr; // The queue is empty
        else
            backPtr->setNext(newNodePtr); // The queue was not empty

        backPtr = newNodePtr; // New node is the last node now
        return true;
    }  // end enqueue

    bool dequeue(T& frntEntry)
    {
        if (isEmpty())
            return false;

        // Save front node and its item
        Node<T>* nodeToDeletePtr = frontPtr;
        frntEntry = nodeToDeletePtr->getItem();

        // Advance front pointer
        frontPtr = frontPtr->getNext();

        // If we just removed the very last node in the queue
        if (nodeToDeletePtr == backPtr) {
            backPtr = nullptr;
            frontPtr = nullptr; // ensure both are null
        }

        delete nodeToDeletePtr; // Free the node memory
        return true;
    }

    bool peek(T& frntEntry) const
    {
        if (isEmpty())
            return false;

        frntEntry = frontPtr->getItem();
        return true;
    }

    // Copy constructor
    LinkedQueue(const LinkedQueue<T>& other)
    {
        frontPtr = backPtr = nullptr;

        Node<T>* current = other.frontPtr;

        while (current) {
            enqueue(current->getItem());
            current = current->getNext();
        }
    }

    void print() const {
        Node<T>* current = frontPtr;

        cout << "Front -> ";
        while (current) {
            cout << current->getItem() << " -> ";
            current = current->getNext();
        }
        cout << "NULL\n";
    }
    
    int getCount() const {
        int count = 0;
        Node<T>* current = frontPtr;
        while (current!=nullptr) {
            count++;
            current = current->getNext();
        }
        return count;
    }

   
    void printIDs() const {
        Node<T>* current = frontPtr;
        while (current!=nullptr) {

            if (current->getItem())
                cout << current->getItem()->GetID();

            if (current->getNext()) cout << ", ";
            current = current->getNext();
        }
        cout << endl;
    }

    ~LinkedQueue()
    {
        // Free all nodes in the queue without requiring T to be default-constructible
        while (frontPtr != nullptr) {
            Node<T>* nodeToDeletePtr = frontPtr;
            frontPtr = frontPtr->getNext();
            delete nodeToDeletePtr;
        }
        backPtr = nullptr;
    }
};

#endif // LINKED_QUEUE_
