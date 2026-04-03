#include "Restaurant.h"
#include <iostream>

using namespace std;

void Restaurant::AddOrder(Order* o)
{
    if (!o) return;
    ORD_TYPE t = o->GetType();
    if (t == TYPE_OV)
        VIPOrders.enqueue(o);
    else if (t == TYPE_OT)
        ColdOrders.enqueue(o);
    else
        NormalOrders.enqueue(o);
}

void Restaurant::CancelOrder(int id)
{
    Order* removed = nullptr;
    if (VIPOrders.removeById(id, removed)) { delete removed; return; }
    if (NormalOrders.removeById(id, removed)) { delete removed; return; }
    if (ColdOrders.removeById(id, removed)) { delete removed; return; }
    if (ReadyOrders.removeById(id, removed)) { delete removed; return; }
}

void Restaurant::PromoteOrder(int id)
{
    Order* removed = nullptr;
    if (NormalOrders.removeById(id, removed))
    {
        // move to VIP queue
        VIPOrders.enqueue(removed);
    }
}

void Restaurant::MoveToReady()
{
    Order* o = nullptr;
    if (VIPOrders.dequeue(o))
    {
        ReadyOrders.enqueue(o);
        return;
    }
    if (NormalOrders.dequeue(o))
    {
        ReadyOrders.enqueue(o);
        return;
    }
    if (ColdOrders.dequeue(o))
    {
        ReadyOrders.enqueue(o);
        return;
    }
}

void Restaurant::FinishOrder()
{
    Order* o = nullptr;
    if (ReadyOrders.dequeue(o))
    {
        FinishedOrders.push(o);
    }
}

void Restaurant::MoveScooterToMaintenance()
{
    Scooter* s = nullptr;
    int pri;
    if (AvailableScooters.dequeue(s, pri))
    {
        MaintenanceScooters.enqueue(s);
        cout << "Scooter -> Maintenance\n";
    }
}

void Restaurant::PrintFinished()
{
    Order* o;

    cout << "\nFinished Orders:\n";

    while (!FinishedOrders.isEmpty())
    {
        FinishedOrders.pop(o);
        if (o) o->Print();
    }
}

DerivedPriQueue<Order*>& Restaurant::getVIP()
{
    return VIPOrders;
}

DerivedQueue<Order*>& Restaurant::getNormal()
{
    return NormalOrders;
}

DerivedQueue<Order*>& Restaurant::getCold()
{
    return ColdOrders;
}

DerivedQueue<Order*>& Restaurant::getReady()
{
    return ReadyOrders;
}

LinkedStack<Order*>& Restaurant::getFinished()
{
    return FinishedOrders;
}

void Restaurant::SimulateStep(int t)
{
    // simple simulation: move one order to ready and finish one
    MoveToReady();
    FinishOrder();
}

bool Restaurant::IsFinished()
{
    return VIPOrders.isEmpty() && NormalOrders.isEmpty() && ColdOrders.isEmpty() && ReadyOrders.isEmpty();
}