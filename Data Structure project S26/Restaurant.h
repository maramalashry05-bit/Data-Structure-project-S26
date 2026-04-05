#pragma once

#include "LinkedQueue.h"
#include "LinkedStack.h"
#include "priQueue.h"
#include "Order.h"
#include "Scooter.h"
#include "DerivedQueue.h"
#include "DerivedpriQueue.h"
#include "Chef.h"
#include "Action.h"
class Restaurant
{
private:
    int TotalServedCount;
    int TotalWaitTime;
    int TotalServiceTime;
    DerivedPriQueue<Order*> VIPOrders;
    DerivedQueue<Order*> NormalOrders;
    DerivedQueue<Order*> ColdOrders;
    DerivedQueue<Order*> ReadyOrders;
    LinkedStack<Order*> FinishedOrders;
    priQueue<Scooter*> AvailableScooters;
    DerivedQueue<Scooter*> MaintenanceScooters;
    DerivedPriQueue<Table*> SmallTables;
    DerivedPriQueue<Table*> MediumTables;
    DerivedPriQueue<Table*> LargeTables;
    DerivedQueue<Order*> ReadyDineIn; 
    DerivedQueue<Order*> ReadyTakeaway; 
    DerivedQueue<Order*> ReadyDelivery;
    DerivedQueue<Scooter*> FastScooters;
    DerivedQueue<Scooter*> SlowScooters;
    priQueue<Order*> FinishedList;

    // Chef pools
    DerivedQueue<Chef*> NormalChefs;
    DerivedQueue<Chef*> SpeedyChefs;

    // Scheduled actions
    LinkedQueue<Action*> Actions;
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
    void AddChef(Chef* c);
    void AssignChefToOrder();

    // Action scheduling
    void AddAction(Action* a);
    void ExecuteActions(int time);
    void AssignOrdersToTables(int currentTime);
    void AssignOrdersToScooters(int currentTime);
    void HandleScooterMaintenance();
    void GenerateFinalReport();
};