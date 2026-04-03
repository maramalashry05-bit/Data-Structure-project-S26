#include "Scooter.h"
Scooter::Scooter(int id)
{
    ID = id;
    available = true;
}
bool Scooter::IsAvailable() const
{
    return available;
}
void Scooter::SetAvailable(bool a)
{
    available = a;
}
void Scooter::Print() const
{
    cout << "Scooter ID: " << ID << endl;
}
