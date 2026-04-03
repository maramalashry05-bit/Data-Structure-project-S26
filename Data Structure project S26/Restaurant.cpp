#include <iostream>
#include "Restaurant.h"

using namespace std;

void Restaurant::AddOrder(Order* o)
{
    pending.enqueue(o);
}

void Restaurant::CancelOrder(int id)
{
    LinkedQueue<Order*> temp;
    Order* o;

    while (!pending.isEmpty())
    {
        pending.dequeue(o);

        if (o->GetID() == id)
        {
            cout << "Cancelled: ";
            o->Print();
            delete o;
        }
        else
            temp.enqueue(o);
    }

    while (!temp.isEmpty())
    {
        temp.dequeue(o);
        pending.enqueue(o);
    }
}

void Restaurant::PromoteOrder(int id)
{
    cout << "Order " << id << " promoted (dummy)\n";
}

void Restaurant::MoveToReady()
{
    Order* o;
    if (pending.dequeue(o))
    {
        ready.enqueue(o);
        cout << "Moved to ready: ";
        o->Print();
    }
}

void Restaurant::FinishOrder()
{
    Order* o;
    if (ready.dequeue(o))
    {
        finished.push(o);
        cout << "Finished: ";
        o->Print();
    }
}

void Restaurant::MoveScooterToMaintenance()
{
    Scooter* s;
    if (scooters.dequeue(s))
    {
        maintenance.enqueue(s);
    }
}

void Restaurant::PrintFinished()
{
    Order* o;
    while (!finished.isEmpty())
    {
        finished.pop(o);
        o->Print();
    }
}
