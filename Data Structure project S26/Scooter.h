#pragma once
#include <iostream>
using namespace std;

class Scooter
{
private:
    int ID;
    bool isAvailable;
    int Speed;
    int TripsDone;
    bool isFast;

public:
    Scooter(int id, int speed, bool fast);
    bool IsAvailable() const;
    void SetAvailable(bool a);
    int GetID() const;
    int GetSpeed() const;
    bool IsFast() const;
    int GetTrips() const;
    void incrementTrips();
    void resetTrips();
    void Print() const;
};