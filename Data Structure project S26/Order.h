#pragma once
#include <iostream>
using namespace std;

enum ORD_TYPE { TYPE_OD, TYPE_OV, TYPE_OT };

class Order
{
private:
    int ID;
    ORD_TYPE type;
    int size;     
    double money;

public:
    Order(int id, ORD_TYPE t, int s, double m);

    int GetID() const;
    ORD_TYPE GetType() const;

    void Print() const;
    friend ostream& operator<<(ostream& out, const Order& o)
    {
        out << "OrderID: " << o.ID;
        return out;
    }
};