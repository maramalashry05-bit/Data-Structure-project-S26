#pragma once
#include <iostream>
#include "Scooter.h"
#include "Chef.h"
#include "Table.h"
using namespace std;

enum ORD_TYPE {
    TYPE_ODG, // Dine-in Grilled (Needs Special Chef)
    TYPE_ODN, // Dine-in Normal
    TYPE_OT,  // Takeaway
    TYPE_OVC, // Delivery Cold
    TYPE_OVG, // Delivery Grilled (Needs Special Chef)
    TYPE_OVN  // Delivery Normal
};

enum ORD_STATUS {
    WAIT,   // Pending Order (not assigned to chef yet)
    COOK,   // Cooking Order (assigned to chef)
    READY,  // Ready Order (done by chef, waiting for delivery/table)
    SRV,    // In-Service Order (on a table or out for delivery)
    DONE    // Finished Order (fully served or delivered)
};
class Order
{
private:
    int ID, size;
    ORD_TYPE type;
    ORD_STATUS status;
    double money;
    int ServiceStartTime;
    int FinishTime;
    int EatingTime;
    int Distance;
    Scooter* assignedScooter;
    Table* assignedTable;
    int ArrivalTime;
    int ReadyTime;
    Chef* myChef;
    bool isUrgent; // Flag to indicate overwait threshold reached

public:
    Order();
    Order(int id, ORD_TYPE t, int s, double m);
    void setStatus(ORD_STATUS s);
    ORD_STATUS getStatus() const;
    void setChef(Chef* c);
    Chef* getChef() const;
    void setUrgent(bool status);
    bool getUrgent() const;
    int GetID() const;
    ORD_TYPE GetType() const;
    string GetTypeString() const;
    void setType(ORD_TYPE t);
    void setMoney(double m);
    double getMoney() const;
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
    void setArrivalTime(int t);
    int getArrivalTime() const;
    void setReadyTime(int t);
    int getReadyTime() const;
    int getServiceStartTime() const;
    int getFinishTime() const;
    int GetNumSeats() const;
};  

