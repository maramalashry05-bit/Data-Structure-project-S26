#include "Restaurant.h"
#include "Chef.h"
#include <iostream>

using namespace std;

//void Restaurant::AddOrder(Order* o)
//{
//    if (!o) return;
//    ORD_TYPE t = o->GetType();
//    if (t == TYPE_OV)
//        VIPOrders.enqueue(o);
//    else if (t == TYPE_OT)
//        ColdOrders.enqueue(o);
//    else
//        NormalOrders.enqueue(o);
//}
//
//void Restaurant::CancelOrder(int id)
//{
//    Order* removed = nullptr;
//    if (VIPOrders.removeById(id, removed)) { delete removed; return; }
//    if (NormalOrders.removeById(id, removed)) { delete removed; return; }
//    if (ColdOrders.removeById(id, removed)) { delete removed; return; }
//    if (ReadyOrders.removeById(id, removed)) { delete removed; return; }
//}
//
//void Restaurant::PromoteOrder(int id)
//{
//    Order* removed = nullptr;
//    if (NormalOrders.removeById(id, removed))
//    {
//        // move to VIP queue
//        VIPOrders.enqueue(removed);
//    }
//}
//
//void Restaurant::MoveToReady()
//{
//    Order* o = nullptr;
//    if (VIPOrders.dequeue(o))
//    {
//        ReadyOrders.enqueue(o);
//        return;
//    }
//    if (NormalOrders.dequeue(o))
//    {
//        ReadyOrders.enqueue(o);
//        return;
//    }
//    if (ColdOrders.dequeue(o))
//    {
//        ReadyOrders.enqueue(o);
//        return;
//    }
//}
//
//void Restaurant::FinishOrder()
//{
//    Order* o = nullptr;
//    if (ReadyOrders.dequeue(o))
//    {
//        FinishedOrders.push(o);
//    }
//}
//
//void Restaurant::MoveScooterToMaintenance()
//{
//    Scooter* s = nullptr;
//    int pri;
//    if (AvailableScooters.dequeue(s, pri))
//    {
//        MaintenanceScooters.enqueue(s);
//        cout << "Scooter -> Maintenance\n";
//    }
//}
//
//void Restaurant::PrintFinished()
//{
//    Order* o;
//
//    cout << "\nFinished Orders:\n";
//
//    while (!FinishedOrders.isEmpty())
//    {
//        FinishedOrders.pop(o);
//        if (o) o->Print();
//    }
//}
//
//DerivedPriQueue<Order*>& Restaurant::getVIP()
//{
//    return VIPOrders;
//}
//
//DerivedQueue<Order*>& Restaurant::getNormal()
//{
//    return NormalOrders;
//}
//
//DerivedQueue<Order*>& Restaurant::getCold()
//{
//    return ColdOrders;
//}
//
//DerivedQueue<Order*>& Restaurant::getReady()
//{
//    return ReadyOrders;
//}
//
//LinkedStack<Order*>& Restaurant::getFinished()
//{
//    return FinishedOrders;
//}
//
//void Restaurant::SimulateStep(int t)
//{
//    // simple simulation: move one order to ready and finish one
//    MoveToReady();
//    FinishOrder();
//}
//
//bool Restaurant::IsFinished()
//{
//    return VIPOrders.isEmpty() && NormalOrders.isEmpty() && ColdOrders.isEmpty() && ReadyOrders.isEmpty();
//}
//
//void Restaurant::AddChef(Chef* c)
//{
//    if (!c) return;
//    if (c->GetType() == CN)
//        NormalChefs.enqueue(c);
//    else
//        SpeedyChefs.enqueue(c);
//}
//
//void Restaurant::AssignChefToOrder()
//{
//    Order* o = nullptr;
//    Chef* ch = nullptr;
//
//    // First try Speedy chefs
//    int speedyCount = SpeedyChefs.getCount();
//    for (int i = 0; i < speedyCount; ++i)
//    {
//        if (!SpeedyChefs.dequeue(ch)) break;
//        if (ch && ch->IsAvailable())
//        {
//            if (ReadyOrders.dequeue(o))
//            {
//                ch->AssignOrder(o, 0); // using 0 as currentTime placeholder
//            }
//        }
//        // put chef back
//        SpeedyChefs.enqueue(ch);
//    }
//
//    // Then try Normal chefs
//    int normalCount = NormalChefs.getCount();
//    for (int i = 0; i < normalCount; ++i)
//    {
//        if (!NormalChefs.dequeue(ch)) break;
//        if (ch && ch->IsAvailable())
//        {
//            if (ReadyOrders.dequeue(o))
//            {
//                ch->AssignOrder(o, 0);
//            }
//        }
//        NormalChefs.enqueue(ch);
//    }
//}
//
//void Restaurant::AssignOrdersToTables(int currentTime) {
//    Order* order;
//    while (ReadyDineIn.peek(order)) {
//        Table* table = nullptr;
//        int groupSize = order->GetNumSeats();
//
//        if (SmallTables.peek(table) && table->getCapacity() >= groupSize) {
//            SmallTables.dequeue(table);
//        }
//        else if (MediumTables.peek(table) && table->getCapacity() >= groupSize) {
//            MediumTables.dequeue(table);
//        }
//        else if (LargeTables.peek(table) && table->getCapacity() >= groupSize) {
//            LargeTables.dequeue(table);
//        }
//
//        if (table) {
//
//            ReadyDineIn.dequeue(order);
//
//            order->SetServiceStartTime(currentTime);
//
//            order->SetFinishTime(currentTime + order->getEatingTime());
//
//            order->setTable(table);
//            InServiceOrders.enqueue(order);
//        }
//        else {
//            break;
//        }
//    }
//}
//
//
//void Restaurant::AssignOrdersToScooters(int currentTime)
//{
//    Order* order;
//    while (ReadyDelivery.peek(order)) {
//        Scooter* scoot = nullptr;
//        if (!FastScooters.isEmpty()) {
//            FastScooters.dequeue(scoot);
//        }
//        else if (!SlowScooters.isEmpty()) {
//            SlowScooters.dequeue(scoot);
//        }
//
//        if (scoot) {
//            ReadyDelivery.dequeue(order);
//
//            order->SetServiceStartTime(currentTime);
//            int travelTime = order->GetDistance() / scoot->GetSpeed();
//            order->SetFinishTime(currentTime + travelTime);
//
//            order->setScooter(scoot);
//            scoot->incrementTrips();
//            InServiceOrders.enqueue(order, order->getFinishTime());
//        }
//        else {
//            break;
//        }
//    }
//}
//void Restaurant::UpdateServiceStatus(int currentTime) {
//    Order* order;
//    int fTime;
//
//
//    while (InServiceOrders.peek(order, fTime)) {
//        if (fTime <= currentTime) {
//            InServiceOrders.dequeue(order, fTime);
//
//
//            if (order->getTable()) {
//                Table* t = order->getTable();
//                t->SetAvailable(true);
//
//                if (t->getCapacity() < 4) SmallTables.enqueue(t);
//                else if (t->getCapacity() < 10) MediumTables.enqueue(t);
//                else LargeTables.enqueue(t);
//            }
//            else if (order->getScooter()) {
//                Scooter* s = order->getScooter();
//                if (s->GetTrips() >= N) MaintenanceScooters.enqueue(s);
//                else {
//                    if (s->IsFast()) FastScooters.enqueue(s);
//                    else SlowScooters.enqueue(s);
//                }
//            }
//
//
//            FinishedList.enqueue(order, fTime);
//        }
//        else {
//            break;
//        }
//    }
//}
//void Restaurant::GenerateFinalReport() {
//
//    cout << "\n--- SIMULATION COMPLETED ---\n";
//    cout << "FT\tID\tAT\tWT\tST" << endl;
//
//    Order* order;
//    int ft;
//    int count = 0;
//    float sumWT = 0, sumST = 0;
//
//
//    while (FinishedList.dequeue(order, ft)) {
//
//        int wt = order->getServiceStartTime() - order->getArrivalTime();
//        int st = order->getFinishTime() - order->getServiceStartTime();
//        cout << ft << "\t" << order->GetID() << "\t" << order->getArrivalTime()
//            << "\t" << wt << "\t" << st << endl;
//
//        sumWT += wt;
//        sumST += st;
//        count++;
//    }
//
//    cout << "------------------------------------------------" << endl;
//    if (count > 0) {
//        cout << "Average Wait Time: " << sumWT / count << endl;
//        cout << "Average Service Time: " << sumST / count << endl;
//    }
//    else {
//        cout << "No orders were completed." << endl;
//    }
//}
//
//
//
//
//// Action scheduling
//void Restaurant::AddAction(Action* a)
//{
//    if (!a) return;
//    Actions.enqueue(a);
//}
//
//void Restaurant::ExecuteActions(int time)
//{
//    LinkedQueue<Action*> temp;
//    Action* act = nullptr;
//
//    while (!Actions.isEmpty())
//    {
//        Actions.dequeue(act);
//        if (!act) continue;
//        if (act->getTime() == time)
//        {
//            act->Execute(this);
//            delete act;
//        }
//        else
//        {
//            temp.enqueue(act);
//        }
//    }
//
//    // restore remaining actions
//    while (!temp.isEmpty())
//    {
//        temp.dequeue(act);
//        Actions.enqueue(act);
//    }
//}