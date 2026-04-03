#pragma once
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
	

};

