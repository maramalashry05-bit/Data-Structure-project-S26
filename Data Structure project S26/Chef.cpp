#include "Chef.h"

Chef::Chef(int id, int sp)
{
    ID = id;
    speed = sp;
    available = true;
}

bool Chef::IsAvailable() const
{
    return available;
}

void Chef::SetAvailable(bool a)
{
    available = a;
}

int Chef::GetID() const
{
    return ID;
}

void Chef::Print() const
{
    cout << "Chef ID: " << ID << " Speed: " << speed << endl;
}