#include <iostream>
#include "Restaurant.h"

using namespace std;

void Restaurant::AddOrder(Order* o)
{
    if (o->GetType() == TYPE_OV) // VIP
    {
        VIPOrders.enqueue(o);   // priority ?????
    }
    else if (o->GetType() == TYPE_OD) // Normal
    {
        NormalOrders.enqueue(o);
    }
    else // Cold
    {
        ColdOrders.enqueue(o);
    }
}

void Restaurant::CancelOrder(int id)
{
        Order* o;

        if (NormalOrders.removeById(id, o))
        {
            cout << "Cancelled: ";
            o->Print();
            delete o;
        }
        else
        {
            cout << "Order not found in Normal\n";
        }
}

void Restaurant::PromoteOrder(int id)
{
    Order* o;

    if (NormalOrders.removeById(id, o))
    {
        VIPOrders.enqueue(o);

        cout << "Promoted: ";
        o->Print();
    }
    else
    {
        cout << "Order not found\n";
    }
}

void Restaurant::MoveToReady()
{
    Order* o;
    int pri;

    if (VIPOrders.dequeue(o))
    {
        ReadyOrders.enqueue(o);
        cout << "VIP -> Ready: ";
        o->Print();
        return;
    }
    if (NormalOrders.dequeue(o))
    {
        ReadyOrders.enqueue(o);
        cout << "Normal -> Ready: ";
        o->Print();
        return;
    }
    if (ColdOrders.dequeue(o))
    {
        ReadyOrders.enqueue(o);
        cout << "Cold -> Ready: ";
        o->Print();
    }
}


void Restaurant::FinishOrder()
{
    Order* o;

    if (ReadyOrders.dequeue(o))
    {
        FinishedOrders.push(o);
        cout << "Finished: ";
        o->Print();
    }
}


void Restaurant::MoveScooterToMaintenance()
{
    Scooter* s;
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
        o->Print();
    }

}
