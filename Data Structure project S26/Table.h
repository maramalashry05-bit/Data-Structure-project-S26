#pragma once
#include <iostream>
using namespace std;

class Table
{
private:
    int ID;
    bool isAvailable;
    int Capacity;

public:
    Table();

    Table(int id);

    bool IsAvailable() const;
    void SetAvailable(bool a);

    void Print() const;
    void setCapacity(int c);
    int getCapacity() const;
    int getID() const;
};