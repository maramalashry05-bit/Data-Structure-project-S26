#include "Scooter.h"
Scooter::Scooter(int id, int speed, bool fast) {
    ID = id;
    Speed = speed;
    isFast = fast;
    isAvailable = true;
    TripsDone = 0;
}
bool Scooter::IsAvailable() const
{
    return isAvailable;
}
void Scooter::SetAvailable(bool a)
{
    isAvailable = a;
}
int Scooter::GetID() const {
    return ID;
}

int Scooter::GetSpeed() const {
    return Speed;
}
bool Scooter::IsFast() const {
    return isFast;
}
int Scooter::GetTrips() const {
    return TripsDone;
}
void Scooter::incrementTrips() {
    TripsDone++;
}

void Scooter::resetTrips() {
    TripsDone = 0;
}
void Scooter::Print() const 
{
    cout << "Scooter ID: " << ID << endl;
}

int Scooter::getMaintenanceFinishTime()
{
    return MaintenanceFinishTime;
}

void Scooter::setMaintenanceFinishTime(int t)
{
    MaintenanceFinishTime = t;
}
