#pragma once
#include "priQueue.h"
#include "Table.h"

class Fit_Tables : public priQueue<Table>
{
public:

    // get best-fit table for given order size
    bool getBestFit(int orderSize, Table& bestTable)
    {
        priQueue<Table> temp;

        Table* current = nullptr;
        int pri;
        bool found = false;

        int bestDiff = INT_MAX;

        while (this->dequeue(*current, pri))
        {
            if (current->getCapacity() >= orderSize)
            {
                int diff = current->getCapacity() - orderSize;

                if (diff < bestDiff)
                {
                    bestDiff = diff;
                    bestTable = *current; // dereference pointer to assign to Table&
                    found = true;
                }
            }

            temp.enqueue(*current, pri);
        }

        // restore queue
        while (temp.dequeue(*current, pri))
        {
            this->enqueue(*current, pri);
        }

        return found;
    }
};