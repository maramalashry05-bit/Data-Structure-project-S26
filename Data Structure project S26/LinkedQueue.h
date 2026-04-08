#pragma once
#ifndef LINKED_QUEUE_
#define LINKED_QUEUE_

#include <iostream> 
using namespace std;
#include "Node.h"
#include "QueueADT.h"

template <typename T>
class LinkedQueue :public QueueADT<T>
{
protected:  //in case you need to inherit for the project
	Node<T>* backPtr;
	Node<T>* frontPtr;
public:


	/////////////////////////////////////////////////////////////////////////////////////////

	LinkedQueue()
	{
		backPtr = nullptr;
		frontPtr = nullptr;

	}
	/////////////////////////////////////////////////////////////////////////////////////////

	/*
	Function: isEmpty
	Sees whether this queue is empty.

	Input: None.
	Output: True if the queue is empty; otherwise false.
	*/
	
	bool isEmpty() const
	{
		return (frontPtr == nullptr);
	}

	/////////////////////////////////////////////////////////////////////////////////////////

	/*Function:enqueue
	Adds newEntry at the back of this queue.

	Input: newEntry .
	Output: True if the operation is successful; otherwise false.
	*/

	
	bool enqueue(const T& newEntry)
	{
		Node<T>* newNodePtr = new Node<T>(newEntry);
		// Insert the new node
		if (isEmpty())	//special case if this is the first node to insert
			frontPtr = newNodePtr; // The queue is empty
		else
			backPtr->setNext(newNodePtr); // The queue was not empty

		backPtr = newNodePtr; // New node is the last node now
		return true;
	}  // end enqueue


	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*Function: dequeue
	Removes the front of this queue. That is, removes the item that was added
	earliest.

	Input: None.
	Output: True if the operation is successful; otherwise false.
	*/

	
	bool dequeue(T& frntEntry)
	{
		if (isEmpty())
			return false;

		Node<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr = frontPtr->getNext();
		// Queue is not empty; remove front
		if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
			backPtr = nullptr;

		// Free memory reserved for the dequeued node
		delete nodeToDeletePtr;

		return true;
	}



	/////////////////////////////////////////////////////////////////////////////////////////

	/*
	Function: peek
	copies the front of this queue to the passed param. The operation does not modify the queue.

	Input: None.
	Output: The front of the queue.
	*/


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
	///////////////////////////////////////////////////////////////////////////////////

	void  print() const {
		Node<T>* current = frontPtr;

		cout << "Front -> ";
		while (current) {
			cout << current->getItem() << " -> ";
			current = current->getNext();
		}
		cout << "NULL\n";
	}


	~LinkedQueue()
	{

		//Free all nodes in the queue
		T *temp;
		while (dequeue(*temp));


	}

#endif
};
