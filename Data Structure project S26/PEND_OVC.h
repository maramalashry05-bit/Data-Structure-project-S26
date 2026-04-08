#pragma once
#include "LinkedQueue.h"
#include "Order.h"

class PEND_OVC : public LinkedQueue<Order*>
{
public:

    bool CancelOrder(int id)
    {
        LinkedQueue<Order*> temp;
        Order* current;
        bool found = false;

        while (this->dequeue(current))
        {
            if (current->GetID() == id)
            {
                found = true;

                
                delete current; // free memory
                continue;
            }

            temp.enqueue(current);
        }

        while (temp.dequeue(current))
        {
            this->enqueue(current);
        }

        return found;
    }

    void PrintDetailed() const
    {
        LinkedQueue<Order*> temp = *this;
        Order* current;

        cout << "PEND_OVC Orders:\n";

        while (temp.dequeue(current))
        {
            cout << "ID: " << current->GetID()
                << " | Type: " << current->GetType()
                << " | Arrival: " << current->getArrivalTime()
                << endl;
        }
    }
};