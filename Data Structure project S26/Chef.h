#pragma once
#include <iostream>
using namespace std;

class Chef
{
private:
    int ID;
    int speed;
    bool available;

public:
    Chef(int id, int sp);

    bool IsAvailable() const;
    void SetAvailable(bool a);

    int GetID() const;

    void Print() const;
};