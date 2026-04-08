#include "Restaurant.h"
#include <iostream>

using namespace std;

// =========================================================================
// Order Management
// =========================================================================

void Restaurant::AddOrder(Order* o)
{
    // Route the order to the correct pending queue based on its type.
    // Note: Adjust the exact enum types based on your definitions in Order.h
    ORD_TYPE type = o->GetType();

    if (type == TYPE_OD) {
        PEND_ODN.enqueue(o);
    }
    else if (type == TYPE_OV) {
        // Example: calculating a priority equation for VIP orders
        int priority = (o->getEatingTime() + o->GetSize()) / o->GetDistance();
        PEND_OVG.enqueue(o, priority);
    }
    else {
        // TYPE_OT
        PEND_OT.enqueue(o);
    }
}

void Restaurant::CancelOrder(int id)
{
    // Attempt to cancel from PEND_OVC (using your custom class method)
    bool cancelled = Pend_OVC.CancelOrder(id);

    if (cancelled) {
        cout << "Order " << id << " cancelled successfully." << endl;
    }
    else {
        // You would typically also implement a search/remove in PEND_ODN 
        // if normal delivery orders can be cancelled.
        cout << "Order " << id << " not found for cancellation." << endl;
    }
}

void Restaurant::PromoteOrder(int id)
{
    LinkedQueue<Order*> tempQueue;
    Order* currentOrder = nullptr;
    bool found = false;

    // Search through normal pending orders
    while (PEND_ODN.dequeue(currentOrder))
    {
        if (currentOrder->GetID() == id)
        {
            found = true;
            // Promote to VIP: Calculate priority and push to PEND_OVG
            int newPriority = 100; // Replace with actual priority equation
            PEND_OVG.enqueue(currentOrder, newPriority);
        }
        else
        {
            tempQueue.enqueue(currentOrder);
        }
    }

    // Restore the normal queue
    while (tempQueue.dequeue(currentOrder))
    {
        PEND_ODN.enqueue(currentOrder);
    }
}

// =========================================================================
// Lifecycle Phase Transitions
// =========================================================================

void Restaurant::MoveToReady()
{
    // Iterate through cooking orders to see if they are done
    // This requires inspecting Cooking_Orders (which inherits from priQueue)
    // You will need a mechanism to check if current time >= order->getFinishTime()
}

void Restaurant::FinishOrder()
{
    Order* finishedOrder = nullptr;
    int priority;

    // Move completed orders from InServ_Orders to the finished list
    // (Requires checking if current time >= service finish time)
    // Example conceptual loop:
    /*
    while (InServ_Orders.peek(finishedOrder, priority)) {
        if (currentTime >= finishedOrder->getFinishTime()) {
            InServ_Orders.dequeue(finishedOrder, priority);
            // Assuming you add a FinishedList (LinkedStack) to your .h
            // FinishedList.push(finishedOrder);

            TotalServedCount++;
            TotalWaitTime += (finishedOrder->getServiceStartTime() - finishedOrder->getArrivalTime());
            TotalServiceTime += (finishedOrder->getFinishTime() - finishedOrder->getServiceStartTime());
        } else {
            break;
        }
    }
    */
}

void Restaurant::MoveScooterToMaintenance()
{
    Scooter* returningScooter = nullptr;
    int priority;

    // Check scooters that are cutting back
    while (Back_Scooters.peek(returningScooter, priority))
    {
        // Check if it reached the restaurant
        Back_Scooters.dequeue(returningScooter, priority);

        // Check maintenance threshold (e.g., 5 trips)
        if (returningScooter->GetTrips() >= 5)
        {
            Maint_Scooters.enqueue(returningScooter);
        }
        else
        {
            // Back to free scooters
            returningScooter->SetAvailable(true);
            Free_Scooters.enqueue(returningScooter, returningScooter->GetSpeed());
        }
    }
}

// =========================================================================
// Resource Assignment
// =========================================================================

void Restaurant::AddChef(Chef* c)
{
    if (c->GetType() == CN) {
        Free_CN.enqueue(c);
    }
    else {
        Free_CS.enqueue(c);
    }
}

void Restaurant::AssignChefToOrder()
{
    // Example logic: Assign a Normal Chef to a Normal Order
    Order* pendingOrder = nullptr;
    Chef* availableChef = nullptr;

    if (!PEND_ODN.isEmpty() && !Free_CN.isEmpty())
    {
        PEND_ODN.dequeue(pendingOrder);
        Free_CN.dequeue(availableChef);

        // Uses the AssignOrder method you defined in Chef.cpp
        int currentTime = 0; // Replace with actual simulation time variable
        availableChef->AssignOrder(pendingOrder, currentTime);
        pendingOrder->SetServiceStartTime(currentTime);
        pendingOrder->SetFinishTime(availableChef->GetFinishTime());

        // Note: Your Cook_Ords queue is defined as priQueue<Order> (by value).
        // If pendingOrder is a pointer, you will need to dereference it:
        // Cooking_Orders.enqueue(*pendingOrder, priority); 
    }
}

void Restaurant::AssignOrdersToTables(int currentTime)
{
    // Logic using your Fit_Tables class
    Order* readyTableOrder = nullptr;

    if (RDY_OT.peek(readyTableOrder))
    {
        Table* bestTable=nullptr;
        if (Free_Tables.getBestFit(readyTableOrder->GetNumSeats(), *bestTable))
        {
            RDY_OT.dequeue(readyTableOrder);
            readyTableOrder->setTable(bestTable);
            // Move table to busy queue, calculate finish time, etc.
        }
    }
}

void Restaurant::AssignOrdersToScooters(int currentTime)
{
    Order* readyDeliveryOrder = nullptr;
    Scooter* availableScooter = nullptr;
    int scooterPri;

    if (!RDY_OD.isEmpty() && !Free_Scooters.isEmpty())
    {
        RDY_OD.dequeue(readyDeliveryOrder);
        Free_Scooters.dequeue(availableScooter, scooterPri);

        readyDeliveryOrder->setScooter(availableScooter);
        availableScooter->incrementTrips();
        availableScooter->SetAvailable(false);

        // Calculate delivery duration
        int deliveryDuration = readyDeliveryOrder->GetDistance() / availableScooter->GetSpeed();
        readyDeliveryOrder->SetFinishTime(currentTime + deliveryDuration);

        // Add to In-Service priority queue (priority based on finish time)
        InServ_Orders.enqueue(readyDeliveryOrder, -readyDeliveryOrder->getFinishTime());
    }
}

void Restaurant::UpdateServiceStatus(int currentTime)
{
    // Check maintenance scooters: 
    // If a scooter has been in maintenance for its fixed duration, dequeue and reset trips
    Scooter* maintScooter = nullptr;
    if (Maint_Scooters.peek(maintScooter))
    {
        // Example check
        Maint_Scooters.dequeue(maintScooter);
        maintScooter->resetTrips();
        maintScooter->SetAvailable(true);
        Free_Scooters.enqueue(maintScooter, maintScooter->GetSpeed());
    }
}

// =========================================================================
// Action Scheduling & Engine
// =========================================================================

void Restaurant::AddAction(Action* a)
{
    Actions.enqueue(a);
}

void Restaurant::ExecuteActions(int time)
{
    Action* act = nullptr;
    while (Actions.peek(act))
    {
        if (act->getTime() == time)
        {
            Actions.dequeue(act);
            // Assuming your actions have an Execute(this) method
            // act->Execute(this); 
            delete act;
        }
        else
        {
            break; // Actions are usually sorted by time; if this is in the future, stop checking.
        }
    }
}

void Restaurant::PrintFinished()
{
    Finished_orders.printIDsReverse();
}

LinkedStack<Order*>& Restaurant::getFinished()
{
    return Finished_orders;
}

void Restaurant::SimulateStep(int t)
{
    ExecuteActions(t);
    UpdateServiceStatus(t);
    MoveScooterToMaintenance();
    MoveToReady();

    AssignChefToOrder();
    AssignOrdersToTables(t);
    AssignOrdersToScooters(t);

    FinishOrder();
}

bool Restaurant::IsFinished()
{
    // The simulation is finished if all queues are empty
    return (PEND_ODN.isEmpty() && PEND_OVG.isEmpty() && PEND_OT.isEmpty() &&
        RDY_OD.isEmpty() && RDY_OT.isEmpty() /* && RDY_OV.isEmpty() */ &&
        Actions.isEmpty() && InServ_Orders.isEmpty());
}

void Restaurant::GenerateFinalReport()
{
    cout << "================ FINISHED ================" << endl;
    cout << "Total Orders Served: " << TotalServedCount << endl;

    if (TotalServedCount > 0) {
        cout << "Average Wait Time: " << (TotalWaitTime / TotalServedCount) << endl;
        cout << "Average Service Time: " << (TotalServiceTime / TotalServedCount) << endl;
    }
}