#pragma once
#include <iostream>
using namespace std;

class Order;  // Forward declaration

enum CHEF_TYPE
{
    CN,   // Normal 
    CS    // Speedy 
};

class Chef
{
private:
    int ID;
    CHEF_TYPE type;
    int speed;

    bool available;
    int finishTime;
    Order* currentOrder;

public:
    
    Chef(int id, CHEF_TYPE t, int s);

    // Getters
    int GetID() const;
    CHEF_TYPE GetType() const;
    int GetSpeed() const;
    bool IsAvailable() const;
    int GetFinishTime() const;
    Order* GetCurrentOrder() const;

    // Setters
    void SetAvailable(bool a);
    void SetFinishTime(int t);
    void SetCurrentOrder(Order* o);

    // Logic
    void AssignOrder(Order* o, int currentTime);
    void FinishOrder();

    // Print
    void Print() const;
};