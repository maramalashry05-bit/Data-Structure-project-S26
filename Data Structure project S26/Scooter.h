#pragma once
#include <iostream>
using namespace std;

class Scooter
{
private:
    int ID;
    bool available;

public:
    Scooter(int id);

    bool IsAvailable() const;
    void SetAvailable(bool a);

    void Print() const;
};