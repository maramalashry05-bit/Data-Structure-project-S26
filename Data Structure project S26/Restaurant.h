#pragma once

#include "LinkedQueue.h"
#include "LinkedStack.h"
#include "priQueue.h"
#include "Order.h"
#include "Scooter.h"
#include "DerivedQueue.h"
#include "DerivedpriQueue.h"
class Restaurant
{
private:
    DerivedPriQueue<Order*> VIPOrders;
    DerivedQueue<Order*> NormalOrders;
    DerivedQueue<Order*> ColdOrders;
    DerivedQueue<Order*> ReadyOrders;
    LinkedStack<Order*> FinishedOrders;
    priQueue<Scooter*> AvailableScooters;
    DerivedQueue<Scooter*> MaintenanceScooters;

public:
    void AddOrder(Order* o);
    void CancelOrder(int id);
    void PromoteOrder(int id);

    void MoveToReady();
    void FinishOrder();

    void MoveScooterToMaintenance();

    void PrintFinished();
    DerivedPriQueue<Order*>& getVIP();
    DerivedQueue<Order*>& getNormal();
    DerivedQueue<Order*>& getCold();
    DerivedQueue<Order*>& getReady();
    LinkedStack<Order*>& getFinished();
    void SimulateStep(int t);
    bool IsFinished();
};