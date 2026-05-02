#pragma once
#include <iostream>
#include "LinkedQueue.h"
#include "Order.h"

using namespace std;

class PEND_OVC : public LinkedQueue<Order*>
{
public:
   
    bool CancelOrder(int id)
    {
        LinkedQueue<Order*> temp;
        Order* current = nullptr;
        bool found = false;

        // Traverse the queue by dequeuing one by one
        while (this->dequeue(current))
        {
            if (current->GetID() == id)
            {
                found = true;
                delete current; 
                continue;       // Skip adding it to the temp queue
            }

            // If it's not the one we want to cancel, save it in temp
            temp.enqueue(current);
        }

        // Restore the original queue perfectly
        while (temp.dequeue(current))
        {
            this->enqueue(current);
        }

        return found;
    }

  
    void PrintDetailed()
    {
        LinkedQueue<Order*> temp;
        Order* current = nullptr;

        cout << "PEND_OVC Orders:\n";

        
        while (this->dequeue(current))
        {
            cout << "ID: " << current->GetID()
                << " | Type: " << current->GetType()
                << " | Arrival: " << current->getArrivalTime()
                << endl;

            temp.enqueue(current); 
        }

        // Restore the original queue
        while (temp.dequeue(current))
        {
            this->enqueue(current);
        }
    }
};