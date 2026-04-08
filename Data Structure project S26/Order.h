#pragma once
#include <iostream>
#include "Scooter.h"
#include "Table.h"
using namespace std;

enum ORD_TYPE { TYPE_OD, TYPE_OV, TYPE_OT };

class Order
{
private:
    int ID, size;
    ORD_TYPE type;
    double money;
    int ServiceStartTime;
    int FinishTime;
    int EatingTime;
    int Distance;
    Scooter* assignedScooter;
    Table* assignedTable;
    int ArrivalTime;
public:
    Order();
    Order(int id, ORD_TYPE t, int s, double m);

    int GetID() const;
    ORD_TYPE GetType() const;

    void Print() const;
    friend ostream& operator<<(ostream& out, const Order& o);
    int GetSize() const;
    void SetServiceStartTime(int t);
    void SetFinishTime(int t);
    int getEatingTime() const;
    void setTable(Table* table);
    int GetDistance() const;
    void SetDistance(int d);
    void SetEatingTime(int t);
    void setScooter(Scooter* s);
    Scooter* getScooter() const;
    Table* getTable() const;
    void setArrivalTime(int wTime);
    int getArrivalTime() const;
    int getServiceStartTime() const;
    int getFinishTime() const;
    int GetNumSeats() const;
};  

