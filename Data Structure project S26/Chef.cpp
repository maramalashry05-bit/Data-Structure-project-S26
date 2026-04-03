#include "Chef.h"
#include "Order.h"

// ================= Constructor =================
Chef::Chef(int id, CHEF_TYPE t, int s)
{
    ID = id;
    type = t;
    speed = s;

    available = true;
    finishTime = 0;
    currentOrder = nullptr;
}

// ================= Getters =================
int Chef::GetID() const
{
    return ID;
}

CHEF_TYPE Chef::GetType() const
{
    return type;
}

int Chef::GetSpeed() const
{
    return speed;
}

bool Chef::IsAvailable() const
{
    return available;
}

int Chef::GetFinishTime() const
{
    return finishTime;
}

Order* Chef::GetCurrentOrder() const
{
    return currentOrder;
}

// ================= Setters =================
void Chef::SetAvailable(bool a)
{
    available = a;
}

void Chef::SetFinishTime(int t)
{
    finishTime = t;
}

void Chef::SetCurrentOrder(Order* o)
{
    currentOrder = o;
}

// ================= Logic =================
void Chef::AssignOrder(Order* o, int currentTime)
{
    currentOrder = o;
    available = false;

    int prepTime = o->GetSize() / speed;
    if (o->GetSize() % speed != 0)
        prepTime++;

    finishTime = currentTime + prepTime;
}

void Chef::FinishOrder()
{
    currentOrder = nullptr;
    available = true;
}

// ================= Print =================
void Chef::Print() const
{
    cout << "Chef ID: " << ID
        << " | Type: " << (type == CN ? "Normal" : "Speedy")
        << " | Speed: " << speed
        << " | Available: " << (available ? "Yes" : "No")
        << endl;
}