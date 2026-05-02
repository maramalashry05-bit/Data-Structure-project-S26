#pragma once
#include "priQueue.h"
#include "Order.h"


class Cook_Ords : public priQueue<Order*>
{
public:
    bool CancelOrder(int id)
    {
        priQueue<Order*> temp;

        Order* current = nullptr; 
        int pri;
        bool found = false;

        // Traverse priority queue
        while (this->dequeue(current, pri))
        {
            if (current->GetID() == id) 
            {
                found = true;
                delete current; 
                continue;       // Skip adding it to the temp queue
            }
            temp.enqueue(current, pri);
        }

        // Restore queue
        while (temp.dequeue(current, pri))
        {
            this->enqueue(current, pri);
        }

        return found;
    }
};