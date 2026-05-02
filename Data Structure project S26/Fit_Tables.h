#pragma once
#include "priQueue.h"
#include "Table.h"


class Fit_Tables : public priQueue<Table*>
{
public:
  
    bool getBestFit(int orderSize, Table*& bestTable)
    {
        priQueue<Table*> temp;
        Table* current = nullptr;
        int pri;
        bool found = false;
        int bestDiff = INT_MAX;
        Table* actualBestTable = nullptr; 

        // 1. Find the best table
        while (this->dequeue(current, pri))
        {
            if (current->getCapacity() >= orderSize)
            {
                int diff = current->getCapacity() - orderSize;
                if (diff < bestDiff)
                {
                    bestDiff = diff;
                    actualBestTable = current;
                    found = true;
                }
            }
            temp.enqueue(current, pri); 
        }

        // 2. Restore queue
        bool removedChosen = false;
        while (temp.dequeue(current, pri))
        {
        
            if (found && !removedChosen && current == actualBestTable) {
                removedChosen = true; // Skip it! (Do not enqueue back)
            }
            else {
                this->enqueue(current, pri);
            }
        }

        // 3. Assign the result
        if (found) {
            bestTable = actualBestTable;
        }

        return found;
    }
};