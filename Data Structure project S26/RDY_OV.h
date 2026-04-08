#pragma once
#pragma once
#include "LinkedQueue.h"
#include "Order.h"

class RDY_OV : public LinkedQueue<Order>
{
public:
    // Cancel order by ID
    bool CancelOrder(int id)
    {
        LinkedQueue<Order> temp;

        Order *current;
        bool found = false;

        while (this->dequeue(*current)) {
            if (current->GetID() == id) {
                found = true; // skip it (delete)
                continue;
            }
            temp.enqueue(*current);
        }

        // restore queue
        while (temp.dequeue(*current)) {
            this->enqueue(*current);
        }

        return found;
    }
};