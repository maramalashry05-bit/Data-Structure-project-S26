#include "Order.h"

Order::Order(int id, ORD_TYPE t, int s, double m)
{
    ID = id;
    type = t;
    size = s;
    money = m;
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