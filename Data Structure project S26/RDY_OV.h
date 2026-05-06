#pragma once
#include "LinkedQueue.h"
#include "Order.h"
#include "iostream "
using namespace std; 

// Inherit from LinkedQueue<Order*> to store pointers
class RDY_OV : public LinkedQueue<Order*>
{
    
public:
    // Cancel order by ID
    bool CancelOrder(int id)
    {
        LinkedQueue<Order*> temp;

        Order* current = nullptr; 
        bool found = false;
        //Cancel logic
        while (this->dequeue(current)) {
            if (current->GetID() == id) { 
                found = true;
                delete current; 
                continue;
            }
            temp.enqueue(current);
        }

        // restore queue
        while (temp.dequeue(current)) {
            this->enqueue(current);
        }
      

        return found;
    }
  
};