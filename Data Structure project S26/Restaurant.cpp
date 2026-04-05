#include "Restaurant.h"
#include "Chef.h"
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

void Restaurant::AddChef(Chef* c)
{
    if (!c) return;
    if (c->GetType() == CN)
        NormalChefs.enqueue(c);
    else
        SpeedyChefs.enqueue(c);
}

void Restaurant::AssignChefToOrder()
{
    Order* o = nullptr;
    Chef* ch = nullptr;

    // First try Speedy chefs
    int speedyCount = SpeedyChefs.getCount();
    for (int i = 0; i < speedyCount; ++i)
    {
        if (!SpeedyChefs.dequeue(ch)) break;
        if (ch && ch->IsAvailable())
        {
            if (ReadyOrders.dequeue(o))
            {
                ch->AssignOrder(o, 0); // using 0 as currentTime placeholder
            }
        }
        // put chef back
        SpeedyChefs.enqueue(ch);
    }

    // Then try Normal chefs
    int normalCount = NormalChefs.getCount();
    for (int i = 0; i < normalCount; ++i)
    {
        if (!NormalChefs.dequeue(ch)) break;
        if (ch && ch->IsAvailable())
        {
            if (ReadyOrders.dequeue(o))
            {
                ch->AssignOrder(o, 0);
            }
        }
        NormalChefs.enqueue(ch);
    }
}

// Action scheduling
void Restaurant::AddAction(Action* a)
{
    if (!a) return;
    Actions.enqueue(a);
}

void Restaurant::ExecuteActions(int time)
{
    LinkedQueue<Action*> temp;
    Action* act = nullptr;

    while (!Actions.isEmpty())
    {
        Actions.dequeue(act);
        if (!act) continue;
        if (act->getTime() == time)
        {
            act->Execute(this);
            delete act;
        }
        else
        {
            temp.enqueue(act);
        }
    }

    // restore remaining actions
    while (!temp.isEmpty())
    {
        temp.dequeue(act);
        Actions.enqueue(act);
    }





}
//void Restaurant::AssignOrdersToTables(int currentTime) {
//    Order* order;
//    while (ReadyDineIn.peek(order)) {
//        Table* table = nullptr;
//        int groupSize = order->GetNumSeats();
//
//        if (SmallTables.peek(table) && pTbl->getCapacity() >= groupSize) {
//            SmallTables.dequeue(table);
//        }
//        else if (MediumTables.peek(table) && pTbl->getCapacity() >= groupSize) {
//            MediumTables.dequeue(table);
//        }
//        else if (LargeTables.peek(table) && pTbl->getCapacity() >= groupSize) {
//            LargeTables.dequeue(table);
//        }
//
//        if (table) {
//  
//            ReadyDineIn.dequeue(order);
//
//            order->setServStartTime(currentTime);
//
//            order->setFinishTime(currentTime + order->getEatingTime());
//
//            order->setTable(table);
//            InServiceOrders.enqueue(order); 
//        }
//        else {
//            break;
//        }
//    }