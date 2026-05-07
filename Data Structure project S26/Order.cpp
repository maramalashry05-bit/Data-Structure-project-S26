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
    ArrivalTime = 0;
    EatingTime = 0;
    FinishTime = 0;
    ReadyTime = 0;
}

void Order::setStatus(ORD_STATUS s)
{
    status = s;
}

ORD_STATUS Order::getStatus() const
{
    return status;
}

void Order::setChef(Chef* c)
{
    myChef = c;
}

Chef* Order::getChef() const
{
    return myChef;
}

void Order::setUrgent(bool status)
{
    isUrgent = status;
}

bool Order::getUrgent() const
{
    return isUrgent;
}

int Order::GetID() const
{
    return ID;
}

ORD_TYPE Order::GetType() const
{
    return type;
}

string Order::GetTypeString() const
{
   
        switch (type) // Assuming your private variable is named 'type'
        {
        case TYPE_ODG: return "ODG";
        case TYPE_ODN: return "ODN";
        case TYPE_OT:  return "OT";
        case TYPE_OVC: return "OVC";
        case TYPE_OVG: return "OVG";
        case TYPE_OVN: return "OVN";
        default:       return "UNKNOWN";
        }
    
}

void Order::setType(ORD_TYPE t)
{
    type = t;
}

void Order::setMoney(double m)
{
    money = m;
}

double Order::getMoney() const
{
    return money;
}

void Order::Print() const
{
    cout << "Order ID: " << ID << " Type: ";

    switch (type)
    {
    case TYPE_ODG: cout << "ODG"; break;
    case TYPE_ODN: cout << "ODN"; break;
    case TYPE_OT:  cout << "OT";  break;
    case TYPE_OVC: cout << "OVC"; break;
    case TYPE_OVG: cout << "OVG"; break;
    case TYPE_OVN: cout << "OVN"; break;
    default:       cout << "UNKNOWN"; break;
    }

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

void Order::setReadyTime(int t)
{
    ReadyTime = t;
}

int Order::getReadyTime() const
{
    return ReadyTime;
}
