#pragma once
#include "priQueue.h"
template <typename T> 

class DerivedPriQueue : public LinkedQueue<T>
{
public :
bool removeByID(int id, T& removed)
	{
		priQueue<T> temp; 
		T item; 
		int pri; 
		bool found = false; 
		while (!this->isEmpty())
		{
			this->dequeue(item, pri); 

			if (!found && item->GetID == id)
			{
				removed = item;
				found = true;

			}
			else
				temp.enqueue(item, pri); 

		}

		while (!temp.isEmpty())  // for restoring 
		{
			temp.dequeue(item);
			this->enqueue(item);

		}
		return found; 

	}
	void updatePriority(int id, int newPri)
	{
		priQueue<T> temp; 
		T item; 
		int pri; 
		 
		while (!this->isEmpty())
		{
			this->dequeue(item, pri);

			if(item->GetID() == id)
				temp.enqueue(item, newPri);
			else
				temp.enqueue(item, pri);
		}

		
		while (!temp.isEmpty())          // for restoeing 
		{
			temp.dequeue(item, pri);
			this->enqueue(item, pri);
		}
	}
};

