#pragma once

#include "LinkedQueue.h"
#include "LinkedStack.h"
#include "priQueue.h"
#include "Order.h"
#include "Scooter.h"

class Restaurant
{
private:
    LinkedQueue<Order*> pending;
    LinkedQueue<Order*> ready;
    LinkedStack<Order*> finished;

    LinkedQueue<Scooter*> scooters;
    LinkedQueue<Scooter*> maintenance;

public:
    void AddOrder(Order* o);
    void CancelOrder(int id);
    void PromoteOrder(int id);

    void MoveToReady();
    void FinishOrder();

    void MoveScooterToMaintenance();

    void PrintFinished();
};