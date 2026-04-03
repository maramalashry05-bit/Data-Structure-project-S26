#pragma once
#include <iostream>
using namespace std;
#include "LinkedQueue.h"
template <typename T> 

class DerivedQueue : public LinkedQueue<T>
{
public : 
	bool  removeById(int id, T& removed)
	{
		LinkedQueue<T> temp; 
		T current; 
		bool found = false; 
		while (!this->isEmpty())
		{
			this->dequeue(current); 
			if (!found && current->GetID() == id)
			{
				removed = current;
				found = true;

			}
			else
				temp.enqueue(current); 

		}
		while (!temp.isEmpty())  // for restoring 
		{
			temp.dequeue(current); 
			this->enqueue(current); 

		}
		return found; 

	}
	void moveAll(DerivedQueue<T>& target)
	{
		T item;

			while (!this->isEmpty())
			{
				this->dequeue(item); 
				target.enqueue(item);
			}

		
	}

	// print full items (assumes T is pointer type like Order* and operator<< is defined)
	void print()
	{
		Node<T>* temp = this->frontPtr;

		while (temp)
		{
			T it = temp->getItem();
			if (it)
				cout << *it << " ";
			else
				cout << "(null) ";
			temp = temp->getNext();
		}
		cout << endl;
	}

	void printIDs()
	{
		Node<T>* temp = this->frontPtr;
	
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
		Node<T>* temp = this->frontPtr;

		while (temp)
		{
			count++;
			temp = temp->getNext();
		}

		return count;
	}

};

