#pragma once
#ifndef LINKED_STACK_H
#define LINKED_STACK_H

#include "StackADT.h"
#include "Node.h"
#include <iostream>
using namespace std;

template <typename T>
class LinkedStack : public StackADT<T> {
private:
    Node<T>* topPtr; // Pointer to the top of the stack

    // Disable shallow copying to avoid double-free issues
    LinkedStack(const LinkedStack&) = delete;
    LinkedStack& operator=(const LinkedStack&) = delete;

public:
    // Constructor
    LinkedStack() : topPtr(nullptr) {}

    // Checks whether this stack is empty
    bool isEmpty() const override {
        return topPtr == nullptr;
    }

    // Adds a new entry to the top of this stack
    bool push(const T& newEntry) override {
        Node<T>* newNodePtr = new Node<T>(newEntry);
        newNodePtr->setNext(topPtr);
        topPtr = newNodePtr;
        return true;
    }

    // Copies the top item to TopEntry and removes it from the stack
    bool pop(T& TopEntry) override {
        if (isEmpty()) return false;

        Node<T>* nodeToDeletePtr = topPtr;
        TopEntry = topPtr->getItem();
        topPtr = topPtr->getNext();

        // IMPORTANT: We only delete the Node wrapper here!
        // We DO NOT delete the object that `TopEntry` might be pointing to.
        delete nodeToDeletePtr;

        return true;
    }

    // Copies the top item to TopEntry without removing it
    bool peek(T& TopEntry) const override {
        if (isEmpty()) return false;

        TopEntry = topPtr->getItem();
        return true;
    }

    // Print IDs from top (most recent) to bottom
    void printIDsReverse()
    {
        Node<T>* temp = topPtr;

        while (temp)
        {
            T it = temp->getItem();
            if (it)
                cout << it->GetID() << " ";
            else
                cout << "(null) ";
            temp = temp->getNext();
        }
        cout << endl;
    }
    int getCount()
    {
        int count = 0;
        Node<T>* temp = topPtr;   

        while (temp)
        {
            count++;
            temp = temp->getNext();
        }

        return count;
    }

    // Copy Constructor
    LinkedStack(const LinkedStack<T>& other) {
        topPtr = nullptr;

        if (other.topPtr == nullptr)
            return;

        // First copy nodes in reverse order using temp stack
        LinkedStack<T> temp;
        Node<T>* current = other.topPtr;

        while (current) {
            temp.push(current->getItem());
            current = current->getNext();
        }

        // Then restore correct order
        T value;
        while (!temp.isEmpty()) {
            temp.pop(value);
            push(value);
        }
    } 

    void print() const {
        Node<T>* current = topPtr;

        cout << "Top -> ";
        while (current) {
            cout << current->getItem() << " -> ";
            current = current->getNext();
        }
        cout << "NULL\n";
    }



    // Destructor
    ~LinkedStack() override {
        T temp;
        // Pop all items to free the memory used by the Node wrappers
        while (pop(temp));
    }
};

#endif