#include "Order.h"
Order::Order()
{

}
Order::Order(int id, ORD_TYPE t, int s, double m)
{
    ID = id;
    type = t;
    size = s;
    money = m;
    assignedScooter = nullptr;
}

int Order::GetID() const
{
    return ID;
}

ORD_TYPE Order::GetType() const
{
    return type;
}

void Order::Print() const
{
    cout << "Order ID: " << ID << " Type: ";

    if (type == TYPE_OD) cout << "OD";
    else if (type == TYPE_OV) cout << "OV";
    else cout << "OT";

    cout << endl;
}
ostream& operator<<(ostream& out, const Order& o)
{
    out << "OrderID: " << o.GetID();
    return out;
}

int Order::GetSize() const
{
    return size;
}

int Order::getEatingTime() const
{
    return EatingTime;
}
void Order::SetServiceStartTime(int t)
{
    ServiceStartTime = t;
}
void Order::SetFinishTime(int t)
{
    FinishTime = t;
}
void Order::setTable(Table* table)
{
    assignedTable = table;
}

int Order::GetDistance() const
{
    return Distance;
}
void Order::SetEatingTime(int e)
{
    EatingTime = e;
}
void Order::SetDistance(int d)
{
    Distance = d;
}
void Order::setScooter(Scooter* s) {
    assignedScooter = s;
}
Scooter* Order::getScooter() const {
    return assignedScooter;
}
Table* Order::getTable() const {
    return assignedTable;
}
int Order::getServiceStartTime() const {
    return ServiceStartTime;
}

int Order::getFinishTime() const
{
    return FinishTime;
}

int Order::GetNumSeats() const
{
    return size;
}

void Order::setArrivalTime(int aTime) {
    ArrivalTime = aTime;
}
int Order::getArrivalTime() const {
    return ArrivalTime;
}