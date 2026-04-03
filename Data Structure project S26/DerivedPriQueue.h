#pragma once
#include "priQueue.h"
#include <iostream>

template <typename T>
class DerivedPriQueue : public priQueue<T>
{
public:
	// enqueue that doesn't require a priority (used by Restaurant::AddOrder / PromoteOrder)
	bool enqueue(const T& entry)
	{
		int defaultPri = 1; // all VIPs same priority by default
		priQueue<T>::enqueue(entry, defaultPri);
		return true;
	}

	// dequeue that hides the priority parameter (used by Restaurant::MoveToReady)
	bool dequeue(T& topEntry)
	{
		int pri;
		return priQueue<T>::dequeue(topEntry, pri);
	}

	// remove by id (both spellings provided to match call sites)
	bool removeByID(int id, T& removed)
	{
		priQueue<T> temp;
		T item;
		int pri;
		bool found = false;

		while (!this->isEmpty())
		{
			priQueue<T>::dequeue(item, pri);
			if (!found && item->GetID() == id) // call GetID()
			{
				removed = item;
				found = true;
			}
			else
				temp.enqueue(item, pri);
		}

		// restore
		while (!temp.isEmpty())
		{
			temp.dequeue(item, pri);
			priQueue<T>::enqueue(item, pri);
		}

		return found;
	}
	bool removeById(int id, T& removed) { return removeByID(id, removed); }

	// update priority for an item with given id
	void updatePriority(int id, int newPri)
	{
		priQueue<T> temp;
		T item;
		int pri;

		while (!this->isEmpty())
		{
			priQueue<T>::dequeue(item, pri);
			if (item->GetID() == id)
				temp.enqueue(item, newPri);
			else
				temp.enqueue(item, pri);
		}

		while (!temp.isEmpty())
		{
			temp.dequeue(item, pri);
			priQueue<T>::enqueue(item, pri);
		}
	}

	// print contents (assumes T is pointer type like Order* or Scooter*)
	void printIDs()
	{
		priNode<T>* cur = this->head; // head is protected in priQueue
		while (cur)
		{
			int p;
			T it = cur->getItem(p);
			std::cout << it->GetID() << " ";
			cur = cur->getNext();
		}
		std::cout << std::endl;
	}
	int getCount()
	{
		int count = 0;
		priNode<T>* cur = this->head;
		while (cur)
		{
			count++;
			cur = cur->getNext();
		}

		return count;
	}
};